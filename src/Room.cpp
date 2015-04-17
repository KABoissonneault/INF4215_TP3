#include "Room.h"

#include "Tile.h"
#include "TileFloor.h"
#include "TileWall.h"
#include "TileTreasure.h"
#include "TileMonster.h"
#include "Game.h"
#include "Player.h"
#include "DisjointSet.h"

#include <iostream>
#include <random>
#include <chrono>
#include <cmath>

namespace INF4215_TP3
{
    Room::Room(sf::RenderTarget& view, unsigned nSizeX, unsigned nSizeY)
        : m_View(view),
        m_nCurrentSeed{}
    {
        // Place the room in the middle of the window
        setPosition(m_View.mapPixelToCoords(sf::Vector2i(Game::knWindowX/2, Game::knWindowY/2)));

        // Initialize the room tiles to empty tiles
        for(unsigned i = 0; i < nSizeX; ++i)
        {
            std::vector<std::shared_ptr<ITile>> apTiles(nSizeY);
            m_a2pTiles.emplace_back(std::move(apTiles));
        }

        m_apPlayers[0].reset(new Player(*this, Player::ID::Player1, Player::ControllerType::AI_Type1, {0,0}));
        m_apPlayers[1].reset(new Player(*this, Player::ID::Player2, Player::ControllerType::AI_Type2, {0,0}));
    }

    Room::~Room() = default;

    void Room::Clean() noexcept
    {
        for(auto& vecColumn : m_a2pTiles)
        {
            for(auto& pTile : vecColumn)
            {
                pTile.reset();
            }
        }
    }

    void Room::DebugOutput(const std::string& sOutput)
    {
        if(Game::Instance().HasDebug())
        {
            std::cout << sOutput << std::endl;
        }
    }

    void Room::Render(sf::RenderTarget& window)
    {
        for(auto& vecColumn : m_a2pTiles)
        {
            for(auto& pTile : vecColumn)
            {
                if(!pTile)
                {
                    continue;
                }

                window.draw(pTile->GetSprite(), getTransform());
            }
        }

        // Draw players
        GetPlayer(1).Render(window, getTransform());
        GetPlayer(2).Render(window, getTransform());
    }

    bool Room::IsOver() const
    {
        for(const auto& column : m_a2pTiles)
        {
            for(const auto& pTile : column)
            {
                if(pTile->GetTileType() == TileType::Treasure)
                {
                    const auto& treasure = static_cast<const TileTreasure&>(*pTile);
                    if(!treasure.isEmpty())
                        return false;
                }
                else if(pTile->GetTileType() == TileType::Monster)
                {
                    const auto& monster = static_cast<const TileMonster&>(*pTile);
                    if(!monster.isEmpty())
                        return false;
                }
            }
        }

        return true;
    }

    void Room::Restart()
    {
        GetPlayer(1).Restart();
        GetPlayer(2).Restart();

        GenerateRoom(m_nCurrentSeed);
    }

    bool Room::GenerateRoom(size_t nSeed)
    {
        Clean();

        std::default_random_engine engine;
        if(nSeed != knDefaultSeed)
        {
            auto nCurrentSeed = nSeed^(nSeed >> 3);
            engine.seed(nCurrentSeed);
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

        GetPlayer(1).Teleport(GetStartPositionPlayer1());
        GetPlayer(2).Teleport(GetStartPositionPlayer2());

        m_nCurrentSeed = nSeed;

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
                    pTile = std::make_shared<TileFloor>(*this, sf::Vector2i(i, j));
                }
                else
                {
                    pTile = std::make_shared<TileWall>(*this, sf::Vector2i(i, j));
                }
            }
        }
    }

    void Room::GenerateTreasures(std::default_random_engine& engine)
    {
        m_nTotalWeaponStrength = 0;

        std::uniform_int_distribution<unsigned> distTreasureCount( GetTileCount() / 80, GetTileCount() / 40 + GetTileCount() / 80);
        //std::uniform_int_distribution<unsigned> distTreasureValue( , 1.0 );
        std::uniform_int_distribution<unsigned> distTreasureType(1, 100);
        const unsigned knTreasureCount = distTreasureCount(engine);

        DebugOutput( "Generating " + std::to_string(knTreasureCount) + " treasures" );

        for(unsigned i = 0; i < knTreasureCount; ++i)
        {
            // Find a random floor tile to replace with a treasure
            const sf::Vector2i pos = GetRandomFloorTile(engine)->GetPosition();
            auto& pTile = m_a2pTiles[pos.x][pos.y];

            // Roll for the treasure type that's going to be chosen
            const unsigned nTreasureType = distTreasureType(engine);
            if(nTreasureType < 71)
            {
                const unsigned nTreasureValue = 2;/*static_cast<unsigned>(std::max(1.0, std::round(distTreasureValue(engine))));*/
                pTile = std::make_shared<TileTreasure>(*this, pos, nTreasureValue, 0);

                DebugOutput( "\tGenerating treasure with value " + std::to_string(nTreasureValue) );
            }
            else if(nTreasureType < 91)
            {
                const unsigned nWeaponValue = 2;/*static_cast<unsigned>(std::max(1.0, std::round(distTreasureValue(engine))));*/
                pTile = std::make_shared<TileTreasure>(*this, pos, 0, nWeaponValue);

                DebugOutput( "\tGenerating weaponry with value " + std::to_string(nWeaponValue) );

                m_nTotalWeaponStrength += nWeaponValue;
            }
            else
            {
                const unsigned nTreasureValue = 1;/*static_cast<unsigned>(std::max(1.0, std::round(distTreasureValue(engine))))/2;*/
                const unsigned nWeaponValue = 1;/*static_cast<unsigned>(std::max(1.0, std::round(distTreasureValue(engine))))/2;*/
                pTile = std::make_shared<TileTreasure>(*this, pos, nTreasureValue, nWeaponValue);

                DebugOutput( "\tGenerating loot with value {" + std::to_string(nTreasureValue) + ", " + std::to_string(nWeaponValue) + "}" );

                m_nTotalWeaponStrength += nWeaponValue;
            }
        }
    }

    void Room::GenerateMonsters(std::default_random_engine& engine)
    {
        std::uniform_int_distribution<unsigned> distMonsterCount( GetTileCount() / 160, GetTileCount() / 80 + GetTileCount() / 160);
        std::uniform_int_distribution<unsigned> distMonsterValue( 1, std::max(3U, m_nTotalWeaponStrength/2) );
        const unsigned knMonsterCount = distMonsterCount(engine);

        DebugOutput( "Generating " + std::to_string(knMonsterCount) + " monsters" );

        for(unsigned i = 0; i < knMonsterCount; ++i)
        {
            bool bPlaced = false;
            while(!bPlaced)
            {
                // Find a random floor tile to replace with a monster tile
                const sf::Vector2i pos = GetRandomFloorTile(engine)->GetPosition();
                auto& pPotentialTile = m_a2pTiles[pos.x][pos.y];
                std::shared_ptr<ITile> pOldFloorTile{pPotentialTile}; // Cache the old floor tile for restoration in case of failure

                // Try replacing the tile with a wall. If the map remains valid,
                // that means that the current tile does not block any way.
                // Therefore, it is a valid spot for placing a monster
                const unsigned knStrength = distMonsterValue(engine);
                pPotentialTile = std::make_shared<TileMonster>(*this, pos, knStrength);

                const auto bResult = ValidateRoom({TileType::Monster});
                if(bResult)
                {
                    bPlaced = true;

                    DebugOutput( "\tGenerating monster with strength " + std::to_string(knStrength) );
                }
                else
                {
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

    bool Room::ValidateRoom(std::set<TileType> invalidTypes)
    {
        invalidTypes.insert(TileType::Wall);

        const auto nNumElements = GetSizeX() * GetSizeY();
        Utility::DisjointSet anGroup(nNumElements);
        // This set is used for invalid tiles. All invalid tiles should belong to that set
        size_t nInvalidSet = 0xFFFFFFFF;

        for(size_t i = 0; i < GetSizeX(); ++i)
        {
            for(size_t j = 0; j < GetSizeY(); ++j)
            {
                const auto& pITile = m_a2pTiles[i][j];
                const size_t nCurrentTilePos = i*GetSizeY() + j;

                if(!pITile || invalidTypes.find(pITile->GetTileType()) != end(invalidTypes))
                {
                    // If this is the first invalid tile, make it the root of the set
                    // Otherwise, merge it to the invalid set
                    if(nInvalidSet == 0xFFFFFFFF)
                    {
                        nInvalidSet = nCurrentTilePos;
                    }
                    else
                    {
                        anGroup.merge(nInvalidSet, nCurrentTilePos);
                    }

                    continue;
                }
                // If this is a non-solid tile, merge it with adjacent non-solid tiles
                else
                {
                    GroupTile(i, j, anGroup, invalidTypes);
                }
            }
        }

        // If the disjoint set has two sets (invalid set and the rest), then all the
        // walkable tiles are connected
        return anGroup.count() == 2;
    }

    void Room::GroupTile(size_t x, size_t y, Utility::DisjointSet& groups, const std::set<TileType>& invalidSet)
    {
        const auto nCurrentPos = x*GetSizeY() + y;

        if(x != 0)
        {
            if(y != 0) MergeIfValid(nCurrentPos, nCurrentPos - GetSizeY() - 1, groups, invalidSet);
            MergeIfValid(nCurrentPos, nCurrentPos - GetSizeY(), groups, invalidSet);
            if(y != GetSizeY() - 1) MergeIfValid(nCurrentPos, nCurrentPos - GetSizeY() + 1, groups, invalidSet);
        }

        if(y != 0) MergeIfValid(nCurrentPos, nCurrentPos - 1, groups, invalidSet);
        if(y != GetSizeY() - 1) MergeIfValid(nCurrentPos, nCurrentPos + 1, groups, invalidSet);

        if(x != GetSizeX() - 1)
        {
            if(y != 0) MergeIfValid(nCurrentPos, nCurrentPos + GetSizeY() - 1, groups, invalidSet);
            MergeIfValid(nCurrentPos, nCurrentPos + GetSizeY(), groups, invalidSet);
            if(y != GetSizeY() - 1) MergeIfValid(nCurrentPos, nCurrentPos + GetSizeY() + 1, groups, invalidSet);
        }
    }

    void Room::MergeIfValid(size_t source, size_t destination, Utility::DisjointSet& groups, const std::set<TileType>& invalidTypes)
    {
        const auto pTile = GetTile(destination);
        if(pTile && invalidTypes.find(pTile->GetTileType()) == end(invalidTypes))
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
            x = Game::Instance().GetRandom() % GetSizeX();
            y = Game::Instance().GetRandom() % GetSizeY();
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

    const TileFloor* Room::GetRandomFloorTile() const
    {
        return const_cast<Room* const>(this)->GetRandomFloorTile();
    }

    const ITile* Room::GetRandomTreasureOrMonster() const
    {
        size_t x;
        size_t y;
        do
        {
            x = Game::Instance().GetRandom() % GetSizeX();
            y = Game::Instance().GetRandom() % GetSizeY();
        }while(GetTile(x, y)->GetTileType() != TileType::Monster && GetTile(x, y)->GetTileType() != TileType::Treasure);

        return GetTile(x, y);
    }

    const Player& Room::GetOtherPlayer(const Player& player) const noexcept
    {
        return *m_apPlayers[knPlayerCount - static_cast<unsigned>(player.GetID())];
    }
}
