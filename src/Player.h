#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <list>
#include <iostream>

namespace INF4215_TP3
{
    class Action;
    class IController;
    class Room;
    class Trail;

    class Player
    {
    public:
        enum class ID : unsigned
        {
            Player1 = 1,
            Player2 = 2
        };

        enum class ControllerType
        {
            Input,
            AI_Type1,
            AI_Type2
        };

        Player(const Room&, ID, ControllerType, const sf::Vector2i& initialPosition);
        ~Player();

        void Render(sf::RenderTarget&, const sf::Transform&);
        void Restart();

        std::unique_ptr<Action> ChooseAction();
        void ExecuteAction(const Action&);

        void Stun(unsigned nTurnCount) noexcept;
        bool IsStunned() const noexcept
        {
            return m_nStunTurnCount > 0;
        }

        const sf::Sprite& GetSprite() const noexcept
        {
            return m_Sprite;
        }

        ID GetID() const noexcept
        {
            return m_eID;
        }

        const Room& GetRoom() const noexcept
        {
            return m_Room;
        }

        const sf::Vector2i& GetPosition() const noexcept
        {
            return m_Position;
        }

        void AddTreasure(unsigned n) noexcept;
        // Returns the amount of treasure that was actually removed from the player
        unsigned RemoveTreasure(unsigned n) noexcept;

        void AddWeapon(unsigned n) noexcept;

        unsigned GetTreasure() const noexcept
        {
            return m_nTreasureCount;
        }

        unsigned GetWeapon() const noexcept
        {
            return m_nWeaponCount;
        }

        void Move(const sf::Vector2i&);
        void Teleport(const sf::Vector2i&);

private:
        void AddTrail(const Action&);
        void CleanTrails();

        std::list<Trail*> m_queueTrails;
        const Room& m_Room;
        sf::Vector2i m_Position;
        sf::Texture m_Texture;
        sf::Sprite m_Sprite;
        std::unique_ptr<IController> m_pController;
        const ID m_eID;

        unsigned m_nStunTurnCount;
        unsigned m_nTreasureCount;
        unsigned m_nWeaponCount;

    };
}
