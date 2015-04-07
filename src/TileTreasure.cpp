#include "TileTreasure.h"

#include "Player.h"

#include <mutex>

namespace INF4215_TP3
{
    sf::Texture TileTreasure::s_textureTreasure;
    sf::Texture TileTreasure::s_textureWeapon;
    sf::Texture TileTreasure::s_textureTreasureWeapon;
    sf::Texture TileTreasure::s_textureEmpty;

    namespace
    {
        std::once_flag s_flagLoad;
    }

    TileTreasure::TileTreasure(const Room& room, const sf::Vector2i& position, unsigned treasureValue, unsigned weaponValue)
        : ITile{room, position, TileType::Treasure},
        m_nTreasureValue{treasureValue},
        m_nWeaponValue{weaponValue}
    {
        std::call_once(s_flagLoad, [] ()
        {
           s_textureTreasure.loadFromFile("src/Treasure_Tile.png");
           s_textureWeapon.loadFromFile("src/Weapon_Tile.png");
           s_textureTreasureWeapon.loadFromFile("src/TreasureWeapon_Tile.png");
           s_textureEmpty.loadFromFile("src/Empty_Tile.png");
        });

        if(treasureValue > 0 && weaponValue > 0)
        {
            SetTexture(s_textureTreasureWeapon);
        }
        else if(treasureValue > 0)
        {
            SetTexture(s_textureTreasure);
        }
        else if(weaponValue > 0)
        {
            SetTexture(s_textureWeapon);
        }
        else
        {
            SetEmpty();
        }

    }

    void TileTreasure::OnEnter(Player& player)
    {
        if(!m_bEmpty)
        {
            if(m_nTreasureValue)
                player.AddTreasure(m_nTreasureValue);
            if(m_nWeaponValue)
                player.AddWeapon(m_nWeaponValue);

            SetEmpty();
        }
    }

    void TileTreasure::SetEmpty()
    {
        SetTexture(s_textureEmpty);
        m_bEmpty = true;
    }

}
