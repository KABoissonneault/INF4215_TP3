#pragma once

#include <SFML/Graphics.hpp>

namespace INF4215_TP3
{
    class Player;

    class ITile
    {
    protected:
        ITile(const sf::Texture& texture, const sf::Vector2f& position);

    public:
        virtual ~ITile() = default;
        virtual bool isSolid() const noexcept = 0;
        virtual void OnEnter(Player& player) = 0;
        // Returns true if a trap was detected on this tile
        // Can use random probabilities
        virtual bool OnDetectTrap() = 0;

        const sf::Sprite& GetSprite() const
        {
            return m_sprite;
        }

    protected:
        sf::Sprite m_sprite;
    };
}
