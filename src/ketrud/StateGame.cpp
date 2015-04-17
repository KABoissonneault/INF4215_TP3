#include "ketrud/StateGame.h"

#include "Player.h"
#include "Room.h"
#include "Tile.h"
#include "TileTreasure.h"
#include "TileMonster.h"

#include <SFML/System/Vector2.hpp>


namespace INF4215_TP3
{
    namespace Ketrud
    {
        struct PlayerInfo
        {
            sf::Vector2i Position;
            unsigned TreasureCount;
            unsigned WeaponCount;
        };

        StateGame::StateGame(const Room& room, const Player& player/*, const Player& otherPlayer*/)
            : m_pPlayer{new PlayerInfo{player.GetPosition(), player.GetTreasure(), player.GetWeapon()}}//,
            //m_pOtherPlayer{new PlayerInfo{otherPlayer.GetPosition(), otherPlayer.GetTreasure(), otherPlayer.GetWeapon()}}
        {
            for(size_t i = 0; i < room.GetTileCount(); ++i)
            {
                const auto pTile = room.GetTile(i);

                if(!pTile)
                    continue;

                if(pTile->GetTileType() == TileType::Treasure)
                {
                    auto pTreasure = std::make_shared<TileTreasure>(static_cast<const TileTreasure&>(*pTile));
                    m_vecTiles.push_back(pTreasure);
                }
                else if(pTile->GetTileType() == TileType::Monster)
                {
                    auto pMonster = std::make_shared<TileMonster>(static_cast<const TileMonster&>(*pTile));
                    m_vecTiles.push_back(pMonster);
                }
            }
        }

        StateGame::~StateGame() = default;

        bool StateGame::operator==(const StateGame& other) const
        {
            if(m_vecTiles.size() != other.m_vecTiles.size())
                return false;

            for(size_t i = 0; i < m_vecTiles.size(); ++i)
            {
                auto pTile = m_vecTiles[i];
                auto pOtherTile = other.m_vecTiles[i];

                if(pTile->GetTileType() != pOtherTile->GetTileType())
                    return false;

                if(pTile->GetTileType() == TileType::Treasure)
                {
                    auto pTreasure = static_cast<const TileTreasure*>(pTile.get());
                    auto pOtherTreasure = static_cast<const TileTreasure*>(pOtherTile.get());

                    if(pTreasure->GetTreasureValue() != pOtherTreasure->GetTreasureValue() ||
                       pTreasure->GetWeaponValue() != pOtherTreasure->GetWeaponValue())
                        return false;
                }
                else if(pTile->GetTileType() == TileType::Monster)
                {
                    auto pMonster = static_cast<const TileMonster*>(pTile.get());
                    auto pOtherMonster = static_cast<const TileMonster*>(pOtherTile.get());

                    if(pMonster->GetTreasureValue() != pOtherMonster->GetTreasureValue() ||
                       pMonster->GetStrength() != pOtherMonster->GetStrength())
                        return false;
                }
            }

            if(GetPlayerInfo().Position != other.GetPlayerInfo().Position ||
               GetPlayerInfo().TreasureCount != other.GetPlayerInfo().TreasureCount ||
               GetPlayerInfo().WeaponCount != other.GetPlayerInfo().WeaponCount)
                return false;

            /*
            if(GetOtherPlayerInfo().Position != other.GetOtherPlayerInfo().Position ||
               GetOtherPlayerInfo().TreasureCount != other.GetOtherPlayerInfo().TreasureCount ||
               GetOtherPlayerInfo().WeaponCount != other.GetOtherPlayerInfo().WeaponCount)
                return false;
            */

            return true;
        }

        bool StateGame::IsTerminal() const
        {
            for(auto pTile : m_vecTiles)
            {
                if(pTile->GetTileType() == TileType::Treasure)
                {
                    if(!static_cast<const TileTreasure*>(pTile.get())->isEmpty())
                        return false;
                }
                else if(pTile->GetTileType() == TileType::Monster)
                {
                    if(!static_cast<const TileMonster*>(pTile.get())->isEmpty())
                        return false;
                }
            }

            return true;
        }
    }
}

namespace std
{
    size_t hash<INF4215_TP3::Ketrud::StateGame>::operator()(const INF4215_TP3::Ketrud::StateGame& state) const noexcept
    {
        size_t nRetHash = 0;
        for(size_t i = 0; i < state.GetTiles().size(); ++i)
        {
            auto pTile = state.GetTiles()[i];
            if(!pTile)
            {
                nRetHash ^=  0x1010101010;
                nRetHash <<= 1;
            }
            if(pTile->GetTileType() == INF4215_TP3::TileType::Treasure)
            {
                auto pTreasure = static_cast<const INF4215_TP3::TileTreasure*>(pTile.get());
                nRetHash ^= pTreasure->GetTreasureValue();
                nRetHash ^= pTreasure->GetWeaponValue();
                nRetHash <<= 2;
            }
            else if(pTile->GetTileType() == INF4215_TP3::TileType::Monster)
            {
                auto pMonster = static_cast<const INF4215_TP3::TileMonster*>(pTile.get());
                nRetHash ^= pMonster->GetTreasureValue();
                nRetHash ^= pMonster->GetStrength();
                nRetHash <<= 4;
            }
        }

        nRetHash ^= state.GetPlayerInfo().Position.x;
        nRetHash ^= state.GetPlayerInfo().Position.y;
        nRetHash ^= state.GetPlayerInfo().TreasureCount;
        nRetHash ^= state.GetPlayerInfo().WeaponCount;
        nRetHash >>= 3;

        /*
        nRetHash ^= state.GetOtherPlayerInfo().Position.x;
        nRetHash ^= state.GetOtherPlayerInfo().Position.y;
        nRetHash ^= state.GetOtherPlayerInfo().TreasureCount;
        nRetHash ^= state.GetOtherPlayerInfo().WeaponCount;
        nRetHash <<= 1;
        */

        return nRetHash;
    }
}
