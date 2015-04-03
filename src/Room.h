#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

namespace INF4215_TP3
{
    class ITile;

    class Room
    {
    public:
        Room(unsigned nSizeX = 5, unsigned nSizeY = 5);
        ~Room();

        void Render(sf::RenderTarget&);

        size_t GetSizeX() const noexcept
        {
            return m_a2pTiles.size();
        }

        size_t GetSizeY() const noexcept
        {
            return m_a2pTiles[0].size();
        }

        void GenerateRoom(sf::RenderTarget& view, size_t seed = knDefaultSeed);
        static const size_t knDefaultSeed = 0;

        ITile* GetTile(size_t x, size_t y)
        {
            return m_a2pTiles[x][y];
        }
    private:
        std::vector<std::vector<ITile*>> m_a2pTiles;
    };
}
