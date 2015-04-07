#pragma once

#include "Tile.h"

namespace INF4215_TP3
{
    class TileTreasure : public ITile
    {
    public:
        TileTreasure(const Room&, const sf::Vector2i&, unsigned treasureValue, unsigned weaponValue);
        virtual ~TileTreasure() = default;

        virtual bool isSolid() const noexcept override
        {
            return false;
        }

        virtual void OnEnter(Player&) override;

        unsigned GetTreasureValue() const noexcept
        {
            return m_nTreasureValue;
        }

        unsigned GetWeaponValue() const noexcept
        {
            return m_nWeaponValue;
        }

    private:
        void SetEmpty();

        static sf::Texture s_textureTreasure;
        static sf::Texture s_textureWeapon;
        static sf::Texture s_textureTreasureWeapon;
        static sf::Texture s_textureEmpty;

        const unsigned m_nTreasureValue;
        const unsigned m_nWeaponValue;
        bool m_bEmpty = false;
    };
}
