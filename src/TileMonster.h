#pragma once

#include "Tile.h"

namespace INF4215_TP3
{
    class TileMonster : public ITile
    {
    public:
        TileMonster(const Room&, const sf::Vector2i&, unsigned strength);
        virtual ~TileMonster() = default;

        virtual bool isSolid() const noexcept override
        {
            return false;
        }

        virtual void OnEnter(Player&) override
        {

        }

        void Defeat();

        bool isEmpty() const noexcept
        {
            return m_bEmpty;
        }

        void AddTreasure(unsigned treasure) noexcept
        {
            m_nTreasureValue += treasure;
        }

        // For when the guy is victorious
        unsigned GetTreasureValue() const noexcept
        {
            return m_nTreasureValue;
        }

        unsigned GetStrength() const noexcept
        {
            return m_nWeaponValue;
        }

    private:
        const sf::Texture& GetTexture();

        static sf::Texture s_textureMonster;
        static sf::Texture s_textureEmpty;

        unsigned m_nTreasureValue;
        const unsigned m_nWeaponValue;
        bool m_bEmpty = false;
    };
}
