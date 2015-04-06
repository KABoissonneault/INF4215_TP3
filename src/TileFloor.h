#pragma once

#include "Tile.h"

namespace INF4215_TP3
{
    class TileFloor : public ITile
    {
    public:
        TileFloor(const sf::Vector2f&);
        virtual ~TileFloor() = default;

        virtual bool isSolid() const noexcept override
        {
            return false;
        }

        virtual bool isTreasure() const noexcept override
        {
            return false;
        }

        virtual void OnEnter(Player&) override
        {

        }

        virtual bool OnDetectTrap() override
        {
            return false;
        }

    private:
        static sf::Texture& GetTexture();
        static sf::Texture s_texture;
    };
}
