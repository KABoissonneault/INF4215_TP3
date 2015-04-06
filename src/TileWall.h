#pragma once

#include "Tile.h"

namespace INF4215_TP3
{
    class TileWall : public ITile
    {
    public:
        TileWall(const sf::Vector2f&);
        virtual ~TileWall() = default;

        virtual bool isSolid() const noexcept override
        {
            return true;
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
