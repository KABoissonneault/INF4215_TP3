#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

namespace INF4215_TP3
{
    class Action;
    class IController;
    class Room;

    class Player
    {
    public:
        enum class ID
        {
            Player1,
            Player2
        };

        enum class ControllerType
        {
            Input,
            AI_Type1,
            AI_Type2
        };

        Player(Room&, ID, ControllerType, const sf::Vector2i& initialPosition);

        std::unique_ptr<Action> GetAction();

        void Stun(unsigned nTurnCount) noexcept;

        const sf::Sprite& GetSprite() const noexcept
        {
            return m_Sprite;
        }

        ID GetPosition() const noexcept
        {
            return m_eID;
        }

    private:
        void SetPosition(const sf::Vector2i&);

        Room& m_Room;
        sf::Vector2i m_Position;
        sf::Texture m_Texture;
        sf::Sprite m_Sprite;
        //std::unique_ptr<IController> m_pController;
        unsigned m_nStunTurnCount;
        ID m_eID;
    };
}
