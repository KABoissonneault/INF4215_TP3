#pragma once

#include "Tile.h"

namespace INF4215_TP3
{
    class TileWall : public ITile
    {
    public:
        TileWall(const Room&, const sf::Vector2i&);
        virtual ~TileWall() = default;

        virtual bool isSolid() const noexcept override
        {
            return true;
        }

        virtual void OnEnter(Player&) override
        {

        }

    private:
        static sf::Texture& GetTexture();
        static sf::Texture s_texture;
    };
}
