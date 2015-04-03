#include "TileFloor.h"

#include <mutex>

namespace INF4215_TP3
{

    namespace
    {
        std::once_flag s_flagLoad;
    }

    sf::Texture TileFloor::s_texture;

    TileFloor::TileFloor(const sf::Vector2f& pos)
        : ITile(GetTexture(), pos)
    {

    }

    sf::Texture& TileFloor::GetTexture()
    {
        std::call_once(s_flagLoad, [] () { s_texture.loadFromFile("src/Floor_Tile.png"); });
        return s_texture;
    }
}
