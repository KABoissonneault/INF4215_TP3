#include "TileMonster.h"

#include <mutex>

namespace  INF4215_TP3
{
    sf::Texture TileMonster::s_textureMonster;
    sf::Texture TileMonster::s_textureEmpty;

    namespace
    {
        std::once_flag s_flagLoad;
    }

    TileMonster::TileMonster(const Room& room, const sf::Vector2i& pos, unsigned strength)
        : ITile(GetTexture(), room, pos, TileType::Monster),
        m_nTreasureValue(strength),
        m_nWeaponValue(strength)
    {

    }

    void TileMonster::Defeat()
    {
        m_bEmpty = true;
        SetTexture(s_textureEmpty);
    }

    const sf::Texture& TileMonster::GetTexture()
    {
        std::call_once(s_flagLoad, [] ()
        {
           s_textureMonster.loadFromFile("src/Monster_Tile.png");
           s_textureEmpty.loadFromFile("src/Empty_Tile.png");
        });

        return s_textureMonster;
    }
}
