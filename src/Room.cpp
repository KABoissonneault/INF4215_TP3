#include "Room.h"

#include "Tile.h"
#include "TileFloor.h"
#include "TileWall.h"
#include "TileTreasure.h"
#include "TileMonster.h"
#include "Game.h"
#include "DisjointSet.h"

#include <iostream>
#include <random>
#include <chrono>
#include <cmath>

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

    void Room::DebugOutput(const std::string& sOutput)
    {
        if(Game::Instance().HasBuildDebug())
        {
            std::cout << sOutput << std::endl;
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

    bool Room::GenerateRoom(size_t nSeed)
    {
        Clean();

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

        // Place floors and walls
        GenerateFloorsAndWalls(engine);

        // Validate room now. If a floor tile is isolated
        // from the rest, this seed is invalid
        const auto bResult = ValidateRoom();
        if(!bResult)
            return false;

        // Place some treasures
        GenerateTreasures(engine);

        // Place some monsters
        GenerateMonsters(engine);

        // Place players at two random valid spots
        GeneratePlayerSpawns(engine);

        return true;
    }

    void Room::GenerateFloorsAndWalls(std::default_random_engine& engine)
    {
        for(size_t i = 0; i < GetSizeX(); ++i)
        {
            auto& vecColumn = m_a2pTiles[i];
            for(size_t j = 0; j < GetSizeY(); ++j)
            {
                auto& pTile = vecColumn[j];

                std::uniform_int_distribution<unsigned> distribution(1, 100);
                const auto tileChoice = distribution(engine);

                if(tileChoice < 51)
                {
                    pTile = new TileFloor(*this, sf::Vector2i(i, j));
                }
                else
                {
                    pTile = new TileWall(*this, sf::Vector2i(i, j));
                }
            }
        }
    }

    void Room::GenerateTreasures(std::default_random_engine& engine)
    {
        std::normal_distribution<> distTreasureCount( static_cast<double>(GetTileCount()) / 40.0, static_cast<double>(GetTileCount()) / 80.0);
        std::normal_distribution<> distTreasureValue( 3.0, 1.0 );
        std::uniform_int_distribution<unsigned> distTreasureType(1, 100);
        const unsigned knTreasureCount = static_cast<unsigned>(std::max(1.0, std::round(distTreasureCount(engine))));

        DebugOutput( "Generating " + std::to_string(knTreasureCount) + " treasures" );

        for(unsigned i = 0; i < knTreasureCount; ++i)
        {
            // Find a random floor tile to replace with a treasure
            const sf::Vector2i pos = GetRandomFloorTile(engine)->GetPosition();
            auto& pTile = m_a2pTiles[pos.x][pos.y];

            // Free the floor tile
            delete pTile;

            // Roll for the treasure type that's going to be chosen
            const unsigned nTreasureType = distTreasureType(engine);
            if(nTreasureType < 41)
            {
                const unsigned nTreasureValue = static_cast<unsigned>(std::max(1.0, std::round(distTreasureValue(engine))));
                pTile = new TileTreasure(*this, pos, nTreasureValue, 0);

                DebugOutput( "\tGenerating treasure with value " + std::to_string(nTreasureValue) );
            }
            else if(nTreasureType < 81)
            {
                const unsigned nWeaponValue = static_cast<unsigned>(std::max(1.0, std::round(distTreasureValue(engine))));
                pTile = new TileTreasure(*this, pos, 0, nWeaponValue);

                DebugOutput( "\tGenerating weaponry with value " + std::to_string(nWeaponValue) );
            }
            else
            {
                const unsigned nTreasureValue = static_cast<unsigned>(std::max(1.0, std::round(distTreasureValue(engine))))/2;
                const unsigned nWeaponValue = static_cast<unsigned>(std::max(1.0, std::round(distTreasureValue(engine))))/2;
                pTile = new TileTreasure(*this, pos, nTreasureValue, nWeaponValue);

                DebugOutput( "\tGenerating loot with value {" + std::to_string(nTreasureValue) + ", " + std::to_string(nWeaponValue) + "}" );
            }
        }
    }

    void Room::GenerateMonsters(std::default_random_engine& engine)
    {
        std::normal_distribution<> distMonsterCount( static_cast<double>(GetTileCount()) / 80.0, static_cast<double>(GetTileCount()) / 160.0 );
        std::normal_distribution<> distMonsterValue( 4.0, 1.0 );
        const unsigned knMonsterCount = static_cast<unsigned>(std::max(1.0, std::round(distMonsterCount(engine))));

        DebugOutput( "Generating " + std::to_string(knMonsterCount) + " monsters" );

        for(unsigned i = 0; i < knMonsterCount; ++i)
        {
            bool bPlaced = false;
            while(!bPlaced)
            {
                // Find a random floor tile to replace with a monster tile
                const sf::Vector2i pos = GetRandomFloorTile(engine)->GetPosition();
                auto& pPotentialTile = m_a2pTiles[pos.x][pos.y];
                const auto pOldFloorTile = pPotentialTile; // Cache the old floor tile for restoration in case of failure

                // Try replacing the tile with a wall. If the map remains valid,
                // that means that the current tile does not block any way.
                // Therefore, it is a valid spot for placing a monster
                pPotentialTile = new TileFloor(*this, pos);

                const auto bResult = ValidateRoom();
                if(bResult)
                {
                    delete pOldFloorTile;
                    delete pPotentialTile; // Delete the wall that was there
                    const unsigned knStrength = static_cast<unsigned>(std::max(1.0, std::round(distMonsterValue(engine))));
                    pPotentialTile = new TileMonster(*this, pos, knStrength);
                    bPlaced = true;

                    DebugOutput( "\tGenerating monster with strength " + std::to_string(knStrength) );
                }
                else
                {
                    delete pPotentialTile;
                    pPotentialTile = pOldFloorTile;
                }
            }
        }
    }

    void Room::GeneratePlayerSpawns(std::default_random_engine& engine)
    {
        // Player 1 is placed around the top left corner, player 2 in the bottom right corner
        // These positions are surprisingly deterministic with the seed
        int xPos;
        int yPos;

        {
            // Generate position for player 1
            std::normal_distribution<> distPosX1( (static_cast<double>(GetSizeX())/2.0 - 1.0) / 2.0, GetSizeX() / 16.0);
            std::normal_distribution<> distPosY1( (static_cast<double>(GetSizeY())/2.0 - 1.0) / 2.0, GetSizeY() / 16.0);
            do
            {
                xPos = std::max(0, static_cast<int>(std::round(distPosX1(engine))));
                yPos = std::max(0, static_cast<int>(std::round(distPosY1(engine))));
            }while(GetTile(xPos, yPos)->GetTileType() != TileType::Floor);
            m_Player1StartPosition = {xPos, yPos};
        }

        {
            // Generate position for player 2
            std::normal_distribution<> distPosX2( (static_cast<double>(GetSizeX())/2.0 + static_cast<double>(GetSizeX()) - 1.0) / 2.0, GetSizeX() / 16.0);
            std::normal_distribution<> distPosY2( (static_cast<double>(GetSizeY())/2.0 + static_cast<double>(GetSizeY()) - 1.0) / 2.0, GetSizeY() / 16.0);
            do
            {
                xPos = std::min(static_cast<int>(GetSizeX()) - 1, static_cast<int>(std::round(distPosX2(engine))));
                yPos = std::min(static_cast<int>(GetSizeY()) - 1, static_cast<int>(std::round(distPosY2(engine))));
            }while(GetTile(xPos, yPos)->GetTileType() != TileType::Floor);
            m_Player2StartPosition = {xPos, yPos};
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

    TileFloor* Room::GetRandomFloorTile()
    {
        size_t x;
        size_t y;
        do
        {
            x = Game::Instance().GetRandom() % (GetSizeX() - 1);
            y = Game::Instance().GetRandom() % (GetSizeY() - 1);
        }while(GetTile(x, y)->GetTileType() != TileType::Floor);

        return static_cast<TileFloor*>(GetTile(x, y));
    }

    TileFloor* Room::GetRandomFloorTile(std::default_random_engine& engine)
    {
        std::uniform_int_distribution<size_t> distPositionX(0, GetSizeX() - 1);
        std::uniform_int_distribution<size_t> distPositionY(0, GetSizeY() - 1);
        size_t x;
        size_t y;
        do
        {
            x = distPositionX(engine);
            y = distPositionY(engine);
        }while(GetTile(x, y)->GetTileType() != TileType::Floor);

        return static_cast<TileFloor*>(GetTile(x, y));
    }
}
