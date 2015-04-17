#pragma once

#include <SFML/Graphics.hpp>

namespace INF4215_TP3
{
    class Player;
    class Room;

    enum class TileType
    {
        Floor,
        Wall,
        Treasure,
        Monster
    };
    class ITile
    {
    protected:
        ITile(const Room& room, const sf::Vector2i& position, TileType eType);
        ITile(const sf::Texture& texture, const Room& room, const sf::Vector2i& position, TileType eType);
        ITile(const ITile&) = default;
        ITile& operator=(const ITile&) = default;

        void SetTexture(const sf::Texture& texture)
        {
            m_sprite.setTexture(texture);
        }

    public:
        virtual ~ITile() = default;


        virtual bool isSolid() const noexcept = 0;
        bool isTreasure() const noexcept
        {
            return m_eType == TileType::Treasure;
        }
        virtual void OnEnter(Player& player) = 0;

        const sf::Sprite& GetSprite() const
        {
            return m_sprite;
        }

        const sf::Vector2i& GetPosition() const
        {
            return m_Position;
        }

        TileType GetTileType() const
        {
            return m_eType;
        }

    private:
        sf::Sprite m_sprite;
        const sf::Vector2i m_Position;
        const TileType m_eType;
    };
}
