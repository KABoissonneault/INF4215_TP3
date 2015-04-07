#include "TileFloor.h"

#include "Room.h"
#include <mutex>

namespace INF4215_TP3
{

    namespace
    {
        std::once_flag s_flagLoad;
    }

    sf::Texture TileFloor::s_texture;

    TileFloor::TileFloor(const Room& room, const sf::Vector2i& pos)
        : ITile(GetTexture(), room, pos, TileType::Floor)
    {

    }

    sf::Texture& TileFloor::GetTexture()
    {
        std::call_once(s_flagLoad, [] () { s_texture.loadFromFile("src/Floor_Tile.png"); });
        return s_texture;
    }
}
