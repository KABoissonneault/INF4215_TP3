#include "Player.h"

#include "Action.h"
#include "Controller.h"
#include "ControllerInput.h"
#include "Room.h"
#include "Tile.h"
#include "Interpolation.h"
#include "Trail.h"
#include "Game.h"

#include <cmath>
#include <cassert>

namespace INF4215_TP3
{
    Player::Player(const Room& room, ID eID, ControllerType eControlType, const sf::Vector2i& initialPosition)
        : m_Room{room},
        m_eID{eID},
        m_nStunTurnCount{0},
        m_nTreasureCount{0},
        m_nWeaponCount{0}
    {
        // Assert the player was spawned in a valid position
        assert(!m_Room.GetTile(initialPosition)->isSolid());

        m_Texture.loadFromFile(eID == ID::Player1 ? "src/Player1.png" : "src/Player2.png");
        m_Sprite.setTexture(m_Texture);

        m_Position = initialPosition;
        m_Sprite.setPosition( m_Room.GetCoordFromTilePos(m_Position.x, m_Position.y) );


        // TODO: Assign controller
        if(eControlType == ControllerType::Input)
        {
            m_pController.reset( new ControllerInput(*this) );
        }
        // TODO: Other controllers
        else
        {
            assert(false && "Not implemented");
        }
    }

    Player::~Player()
    {
        for(auto pTrail : m_queueTrails)
        {
            delete pTrail;
        }
    }

    void Player::Render(sf::RenderTarget& view, const sf::Transform& context)
    {
        for(auto pTrail : m_queueTrails)
        {
            pTrail->Render(view, context);
        }

        // Interpolate it closer to destination
        m_Sprite.setPosition(Utility::WeightedAverageInterpolate(m_Sprite.getPosition(), m_Room.GetCoordFromTilePos(m_Position.x, m_Position.y), 4));

        view.draw(m_Sprite, context);
    }

    void Player::SetPosition(const sf::Vector2i& pos)
    {
        // Start sprite transition to destination
        // Make sure the sprite starts from the current position in case it hasn't reached it yet
        m_Sprite.setPosition( m_Room.GetCoordFromTilePos(m_Position.x, m_Position.y) );

        // Render will make sure to move the sprite towards the current position

        // now assign the destination
        m_Position = pos;
    }

    std::unique_ptr<Action> Player::ChooseAction()
    {
        // If stunned, return idle action
        if(m_nStunTurnCount > 0)
        {
            --m_nStunTurnCount;
            return std::unique_ptr<Action>(new Action{Action::Direction::Idle});
        }

        // Otherwise, let the controller decide
        return m_pController->ChooseAction();
    }

    void Player::ExecuteAction(const Action& eAction)
    {
        sf::Vector2i newPos = m_Position;
        unsigned direction = static_cast<unsigned>(eAction.ChosenDirection);

        if(direction & static_cast<unsigned>(Action::Direction::Top))
        {
            newPos.y -= 1;
        }
        else if(direction & static_cast<unsigned>(Action::Direction::Bottom))
        {
            newPos.y += 1;
        }

        if(direction & static_cast<unsigned>(Action::Direction::Left))
        {
            newPos.x -= 1;
        }
        else if(direction & static_cast<unsigned>(Action::Direction::Right))
        {
            newPos.x += 1;
        }

        const auto pTile = m_Room.GetTile(newPos);
        if( pTile && !(pTile->isSolid()) )
        {
            // On ajoute une trail
            AddTrail(eAction);

            SetPosition(newPos);
        }
    }

    void Player::AddTrail(const Action& action)
    {
        const unsigned knMaxTrail = Game::Instance().GetMaxTrail();
        if(knMaxTrail == 0)
        {
            return;
        }

        m_queueTrails.push_back(new Trail(*this, action));

        while(m_queueTrails.size() > knMaxTrail)
        {
            delete m_queueTrails.front();
            m_queueTrails.pop_front();
        }
    }

    void Player::Stun(unsigned nTurnCount) noexcept
    {
        m_nStunTurnCount = std::max(m_nStunTurnCount, nTurnCount);
    }
}
