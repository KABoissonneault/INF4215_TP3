#include "Room.h"

#include "Tile.h"
#include "TileFloor.h"
#include "Game.h"

namespace INF4215_TP3
{
    Room::Room(unsigned nSizeX, unsigned nSizeY)
    {
        for(unsigned i = 0; i < nSizeX; ++i)
        {
            auto apTiles = std::vector<ITile*>(nSizeY, nullptr);
            m_a2pTiles.emplace_back(std::move(apTiles));
        }
    }

    Room::~Room()
    {
        for(auto& vecColumn : m_a2pTiles)
        {
            for(auto pTile : vecColumn)
            {
                delete pTile;
            }
        }
    }

    void Room::Render(sf::RenderTarget& window)
    {
        for(size_t i = 0; i < GetSizeX(); ++i)
        {
            auto& vecColumn = m_a2pTiles[i];
            for(size_t j = 0; j < GetSizeY(); ++j)
            {
                auto pTile = vecColumn[j];
                if(!pTile)
                {
                    continue;
                }

                window.draw(pTile->GetSprite());
            }
        }
    }

    void Room::GenerateRoom(sf::RenderTarget& view, size_t nSeed)
    {
        // Temporary generator
        (void)nSeed;

        for(size_t i = 0; i < GetSizeX(); ++i)
        {
            auto& vecColumn = m_a2pTiles[i];
            for(size_t j = 0; j < GetSizeY(); ++j)
            {
                auto& pTile = vecColumn[j];

                const auto& pos = view.mapPixelToCoords(sf::Vector2i((i+1)*32, (j+1)*32));
                pTile = new TileFloor(pos);
            }
        }
    }
}
