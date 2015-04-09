#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

namespace INF4215_TP3
{
    class ITile;
    class TileFloor;
    class Player;

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

        size_t GetTileCount() const noexcept
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

        sf::Vector2f GetCoordFromTilePos(sf::Vector2i pos) const
        {
            return GetCoordFromTilePos(pos.x, pos.y);
        }

        bool GenerateRoom(size_t seed = knDefaultSeed);

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
            return GetTile(pos/GetSizeY(), pos%GetSizeY());
        }

        const ITile* GetTile(size_t x, size_t y) const
        {
            return const_cast<Room* const>(this)->GetTile(x, y);
        }

        const ITile* GetTile(const sf::Vector2i& pos) const
        {
            return const_cast<Room* const>(this)->GetTile(pos);
        }

        const ITile* GetTile(size_t pos) const
        {
            return const_cast<Room* const>(this)->GetTile(pos);
        }

        const sf::Vector2i& GetStartPositionPlayer1() const
        {
            return m_Player1StartPosition;
        }

        const sf::Vector2i& GetStartPositionPlayer2() const
        {
            return m_Player2StartPosition;
        }

        TileFloor* GetRandomFloorTile();

    private:
        void GenerateFloorsAndWalls(std::default_random_engine& engine);
        void GenerateTreasures(std::default_random_engine& engine);
        void GenerateMonsters(std::default_random_engine& engine);
        void GeneratePlayerSpawns(std::default_random_engine& engine);

        bool ValidateRoom();

        void DebugOutput(const std::string&);

        // For seed consistency
        TileFloor* GetRandomFloorTile(std::default_random_engine& engine);

        void GroupTile(size_t x, size_t y, Utility::DisjointSet& groups);
        void MergeIfValid(size_t source, size_t destination, Utility::DisjointSet& groups);

        sf::RenderTarget& m_View;
        std::vector<std::vector<ITile*>> m_a2pTiles;

        sf::Vector2i m_Player1StartPosition;
        sf::Vector2i m_Player2StartPosition;
    };
}
