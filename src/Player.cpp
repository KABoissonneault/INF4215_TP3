#include "Player.h"

#include "Action.h"
#include "Controller.h"
#include "ControllerInput.h"
#include "ControllerInputAlt.h"
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
        m_nWeaponCount{1}
    {
        m_Texture.loadFromFile(eID == ID::Player1 ? "src/Player1.png" : "src/Player2.png");
        m_Sprite.setTexture(m_Texture);

        m_Position = initialPosition;
        m_Sprite.setPosition( m_Room.GetCoordFromTilePos(m_Position.x, m_Position.y) );


        // TODO: Assign controller
        if(eControlType == ControllerType::Input)
        {
            if(eID == ID::Player1)
            {
                m_pController.reset( new ControllerInputAlt(*this) );
            }
            else
            {
                m_pController.reset( new ControllerInput(*this) );
            }

        }
        // TODO: Other controllers
        else
        {
            assert(false && "Not implemented");
        }
    }

    Player::~Player()
    {
        CleanTrails();
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

    void Player::Restart()
    {
        m_nStunTurnCount = 0;
        m_nTreasureCount = 0;
        m_nWeaponCount = 1;

        CleanTrails();
    }

    void Player::Move(const sf::Vector2i& pos)
    {
        // Start sprite transition to destination
        // Make sure the sprite starts from the current position in case it hasn't reached it yet
        m_Sprite.setPosition( m_Room.GetCoordFromTilePos(m_Position.x, m_Position.y) );

        // Render will make sure to move the sprite towards the current position

        // now assign the destination
        m_Position = pos;
    }

    void Player::Teleport(const sf::Vector2i& pos)
    {
        m_Position = pos;

        m_Sprite.setPosition( m_Room.GetCoordFromTilePos(m_Position.x, m_Position.y) );

        CleanTrails();
    }

    void Player::AddTreasure(unsigned n) noexcept
    {
        m_nTreasureCount += n;
        std::cout << "Joueur " << static_cast<unsigned>(m_eID) << ": " << m_nTreasureCount << "(+"
        << n << ") points de tresor" << std::endl;

        m_pController->OnTreasureChange(static_cast<int>(n));
    }

    unsigned Player::RemoveTreasure(unsigned n) noexcept
    {
        const auto knOldTreasureCount = m_nTreasureCount;
        m_nTreasureCount = static_cast<unsigned>(std::max(0, static_cast<int>(m_nTreasureCount) - static_cast<int>(n)));
        const auto knRemovedTreasure = knOldTreasureCount - m_nTreasureCount;

        std::cout << "Joueur " << static_cast<unsigned>(m_eID) << ": " << m_nTreasureCount << "(-"
        << knRemovedTreasure << ") points de tresor" << std::endl;

        m_pController->OnTreasureChange(-static_cast<int>(knRemovedTreasure));

        return knRemovedTreasure;
    }

    void Player::AddWeapon(unsigned n) noexcept
    {
        m_nWeaponCount += n;
        std::cout << "Joueur " << static_cast<unsigned>(m_eID) << ": " << m_nWeaponCount << "(+"
        << n << ") points de force" << std::endl;
    }


    std::unique_ptr<Action> Player::ChooseAction()
    {
        // If stunned, return idle action
        if(IsStunned())
        {
            --m_nStunTurnCount;
            return std::unique_ptr<Action>(new Action{*this, Action::Direction::Idle});
        }

        // Otherwise, let the controller decide
        return m_pController->ChooseAction();
    }

    void Player::ExecuteAction(const Action& eAction)
    {
        if(IsStunned())
            return;

        const sf::Vector2i& newPos = eAction.GetResultingPosition();

        const auto pTile = m_Room.GetTile(newPos);
        if( pTile && !(pTile->isSolid()) )
        {
            // On ajoute une trail
            AddTrail(eAction);

            Move(newPos);
        }
    }

    void Player::OnTurnEnd()
    {
        m_pController->OnTurnEnd();
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

    void Player::CleanTrails()
    {
        for(auto trail : m_queueTrails)
        {
            delete trail;
        }

        m_queueTrails.clear();
    }

    void Player::Stun(unsigned nTurnCount) noexcept
    {
        m_nStunTurnCount = std::max(m_nStunTurnCount, nTurnCount);

        std::cout << "Joueur " << static_cast<unsigned>(m_eID) << ": " << "paralyse pour " << nTurnCount
        << " tours" << std::endl;

        m_pController->OnStun(m_nStunTurnCount);
    }
}
