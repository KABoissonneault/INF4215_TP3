#include "Trail.h"

#include "Player.h"
#include "Action.h"
#include "Room.h"

#include <mutex>

namespace INF4215_TP3
{
    sf::Texture Trail::s_textPlayer1Trail;
    sf::Texture Trail::s_textPlayer2Trail;

    namespace
    {
        std::once_flag s_flagLoad;
    }
    Trail::Trail(const Player& player, const Action& action)
    {
        std::call_once(s_flagLoad,
        [] ()
        {
           s_textPlayer1Trail.loadFromFile("src/Trail1.png");
           s_textPlayer2Trail.loadFromFile("src/Trail2.png");
        });

        const int length = static_cast<int>(Room::knTilePixelLength);
        const unsigned nOutDir = static_cast<unsigned>(action.ChosenDirection);
        sf::Vector2i nextPos = player.GetPosition();
        int xSourceOut;
        int ySourceOut;
        if(nOutDir & static_cast<unsigned>(Action::Direction::Left))
        {
            xSourceOut = 0;
            --nextPos.x;
        }
        else if(nOutDir & static_cast<unsigned>(Action::Direction::Right))
        {
            xSourceOut = 2*length;
            ++nextPos.x;
        }
        else
        {
            xSourceOut = length;
        }

        if(nOutDir & static_cast<unsigned>(Action::Direction::Top))
        {
            ySourceOut = 0;
            --nextPos.y;
        }
        else if(nOutDir & static_cast<unsigned>(Action::Direction::Bottom))
        {
            ySourceOut = 2 * length;
            ++nextPos.y;
        }
        else
        {
            ySourceOut = length;
        }

        const sf::Texture& chosenTexture = player.GetID() == Player::ID::Player1 ? s_textPlayer1Trail : s_textPlayer2Trail;

        m_spriteSourceTrail.setTexture(chosenTexture);
        m_spriteSourceTrail.setTextureRect(sf::IntRect(sf::Vector2i(length, length), sf::Vector2i(length, length)));
        m_spriteSourceTrail.setPosition(player.GetRoom().GetCoordFromTilePos(player.GetPosition()));

        m_spriteSourceOutTrail.setTexture(chosenTexture);
        m_spriteSourceOutTrail.setTextureRect(sf::IntRect(sf::Vector2i(xSourceOut, ySourceOut), sf::Vector2i(length, length)));
        m_spriteSourceOutTrail.setPosition(player.GetRoom().GetCoordFromTilePos(player.GetPosition()));

        m_spriteDestinationInTrail.setTexture(chosenTexture);
        m_spriteDestinationInTrail.setTextureRect(sf::IntRect(sf::Vector2i(2*length - xSourceOut, 2*length - ySourceOut), sf::Vector2i(length, length)));
        m_spriteDestinationInTrail.setPosition(player.GetRoom().GetCoordFromTilePos(nextPos));
    }

    void Trail::Render(sf::RenderTarget& view, const sf::Transform& context)
    {
        view.draw(m_spriteSourceOutTrail, context);
        view.draw(m_spriteDestinationInTrail, context);
        view.draw(m_spriteSourceTrail, context);
    }

}
