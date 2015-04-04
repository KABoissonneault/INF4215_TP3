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
        Room(sf::RenderTarget& view, unsigned nSizeX = 5, unsigned nSizeY = 5);
        ~Room();

        static const unsigned knTilePixelLength = 32;

        void Clean() noexcept;
        void Render(sf::RenderTarget&);

        size_t GetSizeX() const noexcept
        {
            return m_a2pTiles.size();
        }

        size_t GetSizeY() const noexcept
        {
            return m_a2pTiles[0].size();
        }

        size_t TileCount() const noexcept
        {
            return GetSizeX() * GetSizeY();
        }

        sf::Vector2f GetCoordFromTilePos(size_t x, size_t y) const
        {
            int xOffset = static_cast<int>((-static_cast<double>(GetSizeX()) / 2.0 + x) * knTilePixelLength);
            int yOffset = static_cast<int>((-static_cast<double>(GetSizeY()) / 2.0 + y) * knTilePixelLength);
            return m_View.mapPixelToCoords(sf::Vector2i(xOffset, yOffset));
        }

        sf::Vector2f GetCoordFromTilePos(size_t pos) const
        {
            return GetCoordFromTilePos(pos/GetSizeX(), pos%GetSizeY());
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

        ITile* GetTile(const sf::Vector2i& pos)
        {
            return GetTile(pos.x, pos.y);
        }

        ITile* GetTile(size_t pos)
        {
            if(pos >= TileCount())
            {
                return nullptr;
            }

            return GetTile(pos/GetSizeY(), pos%GetSizeY());
        }


    private:
        void GroupTile(size_t x, size_t y, Utility::DisjointSet& groups);
        void MergeIfValid(size_t source, size_t destination, Utility::DisjointSet& groups);

        sf::RenderTarget& m_View;
        std::vector<std::vector<ITile*>> m_a2pTiles;
    };
}
