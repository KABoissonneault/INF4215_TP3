#include "Player.h"

//#include "Action.h"
//#include "Controller.h"

#include "Room.h"
#include "Tile.h"

#include <cmath>
#include <cassert>

namespace INF4215_TP3
{
    Player::Player(Room& room, ID eID, ControllerType eControlType, const sf::Vector2i& initialPosition)
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
        (void)eControlType;
    }

    class Action
    {

    };

    void Player::SetPosition(const sf::Vector2i& pos)
    {
        m_Position = pos;
        m_Sprite.setPosition( m_Room.GetCoordFromTilePos(m_Position.x, m_Position.y) );
    }

    std::unique_ptr<Action> Player::GetAction()
    {
        return nullptr;
    }

    void Player::Stun(unsigned nTurnCount) noexcept
    {
        m_nStunTurnCount = std::max(m_nStunTurnCount, nTurnCount);
    }
}
