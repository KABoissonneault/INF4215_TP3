#include "Tile.h"

namespace INF4215_TP3
{
    ITile::ITile(const sf::Texture& text, const sf::Vector2f& pos)
        : m_sprite(text)
    {
        m_sprite.setPosition(pos);
    }
}
