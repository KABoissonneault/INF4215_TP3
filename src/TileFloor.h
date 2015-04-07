#pragma once

#include "Tile.h"

namespace INF4215_TP3
{
    class TileFloor : public ITile
    {
    public:
        TileFloor(const Room&, const sf::Vector2i&);
        virtual ~TileFloor() = default;

        virtual bool isSolid() const noexcept override
        {
            return false;
        }

        virtual void OnEnter(Player&) override
        {

        }

    private:
        static sf::Texture& GetTexture();
        static sf::Texture s_texture;
    };
}
