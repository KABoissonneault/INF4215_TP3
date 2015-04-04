#include "Room.h"

#include "Tile.h"
#include "TileFloor.h"
#include "TileWall.h"
#include "Game.h"
#include "DisjointSet.h"

#include <random>
#include <chrono>

namespace INF4215_TP3
{
    Room::Room(sf::RenderTarget& view, unsigned nSizeX, unsigned nSizeY)
        : m_View(view)
    {
        // Place the room in the middle of the window
        setPosition(m_View.mapPixelToCoords(sf::Vector2i(Game::knWindowX/2, Game::knWindowY/2)));

        // Initialize the room tiles to empty tiles
        for(unsigned i = 0; i < nSizeX; ++i)
        {
            auto apTiles = std::vector<ITile*>(nSizeY, nullptr);
            m_a2pTiles.emplace_back(std::move(apTiles));
        }
    }

    Room::~Room()
    {
        Clean();
    }

    void Room::Clean() noexcept
    {
        for(auto& vecColumn : m_a2pTiles)
        {
            for(auto& pTile : vecColumn)
            {
                delete pTile;
                pTile = nullptr;
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

                window.draw(pTile->GetSprite(), getTransform());
            }
        }
    }

    void Room::GenerateRoom(size_t nSeed)
    {
        Clean();

        // Temporary generator
        std::default_random_engine engine;
        if(nSeed != knDefaultSeed)
        {
            engine.seed(nSeed);
        }
        else
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            engine.seed(seed);
        }

        for(size_t i = 0; i < GetSizeX(); ++i)
        {
            auto& vecColumn = m_a2pTiles[i];
            for(size_t j = 0; j < GetSizeY(); ++j)
            {
                auto& pTile = vecColumn[j];
                const auto& pos = GetCoordFromTilePos(i, j);
                std::uniform_int_distribution<unsigned> distribution(1, 100);
                const auto tileChoice = distribution(engine);

                if(tileChoice < 51)
                {
                    pTile = new TileFloor(pos);
                }
                else
                {
                    pTile = new TileWall(pos);
                }

            }
        }
    }

    bool Room::ValidateRoom()
    {
        const auto nNumElements = GetSizeX() * GetSizeY();
        Utility::DisjointSet anGroup(nNumElements);
        // This set is used for invalid tiles. All invalid tiles should belong to that set
        size_t nWallSet = 0xFFFFFFFF;

        for(size_t i = 0; i < GetSizeX(); ++i)
        {
            for(size_t j = 0; j < GetSizeY(); ++j)
            {
                ITile* pITile = m_a2pTiles[i][j];
                const size_t nCurrentTilePos = i*GetSizeY() + j;

                if(!pITile || pITile->isSolid())
                {
                    // If this is the first invalid tile, make it the root of the set
                    // Otherwise, merge it to the invalid set
                    if(nWallSet == 0xFFFFFFFF)
                    {
                        nWallSet = nCurrentTilePos;
                    }
                    else
                    {
                        anGroup.merge(nWallSet, nCurrentTilePos);
                    }

                    continue;
                }
                // If this is a non-solid tile, merge it with adjacent non-solid tiles
                else
                {
                    GroupTile(i, j, anGroup);
                }
            }
        }

        // If the disjoint set has two sets (invalid set and the rest), then all the
        // walkable tiles are connected
        return anGroup.count() == 2;
    }

    void Room::GroupTile(size_t x, size_t y, Utility::DisjointSet& groups)
    {
        const auto nCurrentPos = x*GetSizeY() + y;

        if(x != 0)
        {
            if(y != 0) MergeIfValid(nCurrentPos, nCurrentPos - GetSizeY() - 1, groups);
            MergeIfValid(nCurrentPos, nCurrentPos - GetSizeY(), groups);
            if(y != GetSizeY() - 1) MergeIfValid(nCurrentPos, nCurrentPos - GetSizeY() + 1, groups);
        }

        if(y != 0) MergeIfValid(nCurrentPos, nCurrentPos - 1, groups);
        if(y != GetSizeY() - 1) MergeIfValid(nCurrentPos, nCurrentPos + 1, groups);

        if(x != GetSizeX() - 1)
        {
            if(y != 0) MergeIfValid(nCurrentPos, nCurrentPos + GetSizeY() - 1, groups);
            MergeIfValid(nCurrentPos, nCurrentPos + GetSizeY(), groups);
            if(y != GetSizeY() - 1) MergeIfValid(nCurrentPos, nCurrentPos + GetSizeY() + 1, groups);
        }
    }

    void Room::MergeIfValid(size_t source, size_t destination, Utility::DisjointSet& groups)
    {
        const auto pTile = GetTile(destination);
        if(pTile && !pTile->isSolid())
        {
            groups.merge(source, destination);
        }
    }
}
