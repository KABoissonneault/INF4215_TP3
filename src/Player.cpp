#include "Player.h"

#include "Action.h"
#include "Controller.h"
#include "ControllerInput.h"
#include "Room.h"
#include "Tile.h"

#include <cmath>
#include <cassert>

namespace INF4215_TP3
{
    Player::Player(const Room& room, ID eID, ControllerType eControlType, const sf::Vector2i& initialPosition)
        : m_Room{room},
        m_nStunTurnCount{0},
        m_eID{eID}
    {
        // Assert the player was spawned in a valid position
        assert(!m_Room.GetTile(initialPosition)->isSolid());

        m_Texture.loadFromFile(eID == ID::Player1 ? "src/Player1.png" : "src/Player2.png");
        m_Sprite.setTexture(m_Texture);

        SetPosition(initialPosition);

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

    Player::~Player() = default;


    void Player::SetPosition(const sf::Vector2i& pos)
    {
        m_Position = pos;
        m_Sprite.setPosition( m_Room.GetCoordFromTilePos(m_Position.x, m_Position.y) );
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
            SetPosition(newPos);
        }
    }

    void Player::Stun(unsigned nTurnCount) noexcept
    {
        m_nStunTurnCount = std::max(m_nStunTurnCount, nTurnCount);
    }
}
