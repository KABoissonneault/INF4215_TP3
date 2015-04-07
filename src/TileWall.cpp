#include "TileWall.h"

#include <mutex>

namespace INF4215_TP3
{

    namespace
    {
        std::once_flag s_flagLoad;
    }

    sf::Texture TileWall::s_texture;

    TileWall::TileWall(const Room& room, const sf::Vector2i& pos)
        : ITile(GetTexture(), room, pos, TileType::Wall)
    {

    }

    sf::Texture& TileWall::GetTexture()
    {
        std::call_once(s_flagLoad, [] () { s_texture.loadFromFile("src/Wall_Tile.png"); });
        return s_texture;
    }
}
