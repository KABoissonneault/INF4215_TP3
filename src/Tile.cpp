#include "Tile.h"

#include "Room.h"

namespace INF4215_TP3
{
    ITile::ITile(const Room& room, const sf::Vector2i& position, TileType eType)
        : m_Position(position),
        m_eType(eType)
    {
        m_sprite.setPosition(room.GetCoordFromTilePos(position));
    }
    ITile::ITile(const sf::Texture& texture, const Room& room, const sf::Vector2i& position, TileType eType)
        : ITile(room, position, eType)
    {
        SetTexture(texture);
    }
}
