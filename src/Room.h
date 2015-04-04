#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

namespace INF4215_TP3
{
    class ITile;

    namespace Utility
    {
        class DisjointSet;
    }

    class Room : public sf::Transformable
    {
    public:
        Room(unsigned nSizeX = 5, unsigned nSizeY = 5);
        ~Room();

        static const unsigned knTilePixelLength = 32;

        void Render(sf::RenderTarget&);

        size_t GetSizeX() const noexcept
        {
            return m_a2pTiles.size();
        }

        size_t GetSizeY() const noexcept
        {
            return m_a2pTiles[0].size();
        }

        void GenerateRoom(size_t seed = knDefaultSeed);
        bool ValidateRoom();
        static const size_t knDefaultSeed = 0;

        ITile* GetTile(size_t x, size_t y)
        {
            if(x >= GetSizeX() || y >= GetSizeY())
            {
                return nullptr;
            }

            return m_a2pTiles[x][y];
        }

        ITile* GetTile(size_t pos)
        {
            if(pos >= TileCount())
            {
                return nullptr;
            }

            return m_a2pTiles[pos/GetSizeY()][pos%GetSizeY()];
        }

        size_t TileCount() const noexcept
        {
            return GetSizeX() * GetSizeY();
        }
    private:
        void GroupTile(size_t x, size_t y, Utility::DisjointSet& groups);
        void MergeIfValid(size_t source, size_t destination, Utility::DisjointSet& groups);



        std::vector<std::vector<ITile*>> m_a2pTiles;
    };
}
