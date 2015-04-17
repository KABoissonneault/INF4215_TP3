#include "GameHandler.h"

#include "Player.h"
#include "Room.h"
#include "Tile.h"
#include "TileMonster.h"
#include "Action.h"
#include "VectorOperations.h"

#include <iostream>
#include <cassert>
#include <random>
#include <cmath>
#include <thread>
#include <chrono>

namespace INF4215_TP3
{
    Game::GameHandler::GameHandler(Game& game)
        : m_Game(game),
        m_apChosenActions{new std::unique_ptr<Action>[2]},
        m_bInitialized{false}
    {

    }
    Game::GameHandler::~GameHandler() = default;

    void Game::GameHandler::Initialize()
    {
        m_pRoom.reset(new Room(m_Game.m_MainWindow, m_Game.m_nMapSizeX, m_Game.m_nMapSizeY));

        if(m_Game.m_nWantedSeed == Game::knDefaultSeed)
        {
            size_t nChosenSeed;
            bool bValid;
            do
            {
                nChosenSeed = m_Game.GetRandom();
                bValid = m_pRoom->GenerateRoom(nChosenSeed);
            } while (!bValid);

            std::cout << "La salle fut generee avec le seed: " << nChosenSeed << std::endl;
        }
        else
        {
            if(!m_pRoom->GenerateRoom(m_Game.m_nWantedSeed))
            {
                throw InitializeException("Le seed passe en argument ne donne pas une salle valide");
            }
        }

        m_bInitialized = true;
    }

    bool Game::GameHandler::Update()
    {
        assert(m_bInitialized);

        for(size_t i = 0; i < knPlayerCount; ++i)
        {
            if(!m_apChosenActions[i])
            {
                m_apChosenActions[i] = GetPlayer(i+1).ChooseAction();
            }
        }

        // Si on a choisi deux actions, on va les exécuter
        // On fait cela car le ControllerInput retourne souvent des actions nulles
        if(m_apChosenActions[0] && m_apChosenActions[1])
        {
            ExecuteActions();
        }

        GetPlayer(1).OnTurnEnd();
        GetPlayer(2).OnTurnEnd();





        if(GetRoom().IsOver())
        {
            //Check for winner
            if(GetPlayer(1).GetTreasure() > GetPlayer(2).GetTreasure())
            {
                GetPlayer(1).OnGameEnd(true);
                GetPlayer(2).OnGameEnd(false);
            }
            else
            {
                GetPlayer(1).OnGameEnd(false);
                GetPlayer(2).OnGameEnd(true);
            }

            std::cout << std::endl;
            std::cout << "Partie terminee." << std::endl;
            std::cout << "Gagnant: " << (GetPlayer(1).GetTreasure() > GetPlayer(2).GetTreasure() ? "Joueur 1" :
                GetPlayer(1).GetTreasure() < GetPlayer(2).GetTreasure() ? "Joueur 2" :
                    "Nul!") << std::endl;

            return true;
        }

        return false;
    }

    void Game::GameHandler::Restart()
    {
        GetRoom().Restart();
    }

    void Game::GameHandler::ExecuteActions()
    {
        CheckPlayerEngagements();
        CheckMonsterEngagements();

        GetPlayer(1).ExecuteAction(GetAction(1));
        GetPlayer(2).ExecuteAction(GetAction(2));

        // Execute the tile's action
        GetRoom().GetTile(GetPlayer(1).GetPosition())->OnEnter(GetPlayer(1));
        GetRoom().GetTile(GetPlayer(2).GetPosition())->OnEnter(GetPlayer(2));

        // Reset actions
        m_apChosenActions[0].reset();
        m_apChosenActions[1].reset();
    }

    void Game::GameHandler::CheckPlayerEngagements()
    {
        // Engagement happens if:
        // - A player attempts to move on a tile which currently has a player
        //   - In this case, the opposing player has a chance to move away if they
        //   move in another direction, which depends on the degree difference
        //   ex:
        //   | A | B | C
        //   | 1 | 2 | D
        //   - If player 1 tries to move on 2, and 2 moves to A, then a fight will occur. If
        //   they move to B, then a fight will occur 75% of the time. If they move to C,
        //   50-50. If they walk to D, they will always flee combat
        //   If a player fails to engage that way, they lose their move action
        // - Both players move on the same tile. Combat will always occur, and
        // only the winning player will move on the tile

        const sf::Vector2i& dest1 = GetAction(1).GetResultingPosition();
        const sf::Vector2i& dest2 = GetAction(2).GetResultingPosition();
        const sf::Vector2i& movement1 = (GetRoom().GetTile(dest1) && !GetRoom().GetTile(dest1)->isSolid())
            ? (dest1 - GetPlayer(1).GetPosition())
            : GetPlayer(1).GetPosition();
        const sf::Vector2i& movement2 = (GetRoom().GetTile(dest2) && !GetRoom().GetTile(dest2)->isSolid())
            ? (dest2 - GetPlayer(2).GetPosition())
            : GetPlayer(2).GetPosition();;


        // Case where both players target the same tile
        // Note that the player that wins will keep its action, and might
        // have to fight a monster tile on the same turn
        if(dest1 == dest2)
        {
            MakePlayersFight();
        }
        else if(dest1 == GetPlayer(2).GetPosition())
        {
            const auto bResult = CheckIfFirstEngagesSecond(movement1, movement2);
            if(bResult)
            {
                MakePlayersFight();
            }
            else
            {
                CancelAction(1);
            }
        }
        else if(dest2 == GetPlayer(1).GetPosition())
        {
            const auto bResult = CheckIfFirstEngagesSecond(movement2, movement1);
            if(bResult)
            {
                MakePlayersFight();
            }
            else
            {
                CancelAction(2);
            }
        }
    }

    bool Game::GameHandler::CheckIfFirstEngagesSecond(const sf::Vector2i& movementSource, const sf::Vector2i& movementTarget)
    {
        if(movementTarget == movementSource)
        {
            return false;
        }
        else if(movementTarget == -movementSource)
        {
            return true;
        }
        else
        {
            int nDivergenceScore = 0;
            if(movementTarget.x == movementSource.x)
                ++nDivergenceScore;
            else if(movementTarget.x == -movementSource.x)
                --nDivergenceScore;
            if(movementTarget.y == movementSource.y)
                ++nDivergenceScore;
            else if(movementTarget.y == -movementSource.y)
                --nDivergenceScore;

            switch(nDivergenceScore)
            {
            case -2:
            case -1:
                return true;
            case 0:
                if(!(m_Game.GetRandom()%4))
                    return false;
                else
                    return true;
            case 1:
                if(!(m_Game.GetRandom()%2))
                    return false;
                else
                    return true;
            case 2:
                return false;
            default:
                return false;
            }
        }
    }

    void Game::GameHandler::CheckMonsterEngagements()
    {
        // Check if the players' action brings them on a monster tile
        for (unsigned i = 0; i < knPlayerCount; ++i)
        {
            auto& player = GetPlayer(i+1);
            auto& action = GetAction(i+1);

            const auto& resultingPos = action.GetResultingPosition();
            const auto pResultingTile = m_pRoom->GetTile(resultingPos);
            if(!pResultingTile)
                continue;

            if(pResultingTile->GetTileType() == TileType::Monster)
            {
                auto& tileMonster = *static_cast<TileMonster*>(pResultingTile);
                if(tileMonster.isEmpty())
                    continue;

                FightPlayerMonster(player, tileMonster);
            }
        }
    }

    void Game::GameHandler::MakePlayersFight()
    {
        std::cout << "Joueur 1 (Force: " << GetPlayer(1).GetWeapon() << ") vs Joueur 2 (Force: "
            << GetPlayer(2).GetWeapon() << "). Gagnant: ";

        const auto knResult = Fight(GetPlayer(1).GetWeapon(), GetPlayer(2).GetWeapon());

        if(knResult < 0)
        {
            std::cout << "Joueur 2 (Score: " << -knResult << ")" << std::endl;

            const auto knLostTreasure = Kill( GetPlayer(1), -knResult );
            GetPlayer(2).AddTreasure(knLostTreasure);
        }
        else
        {
            std::cout << "Joueur 1 (Score: " << knResult << ")" << std::endl;

            const auto knLostTreasure = Kill( GetPlayer(2), knResult );
            GetPlayer(1).AddTreasure(knLostTreasure);
        }
    }

    void Game::GameHandler::FightPlayerMonster(Player& player, TileMonster& tileMonster)
    {
        std::cout << "Joueur " << static_cast<unsigned>(player.GetID()) << " (Force: " << player.GetWeapon() << ") vs Monstre (Force: "
        << tileMonster.GetStrength() << "). Gagnant: ";

        const auto knResult = Fight(player.GetWeapon(), tileMonster.GetStrength());

        if(knResult < 0)
        {
            std::cout << "Monstre (Score: " << -knResult << ")" << std::endl;

            const unsigned knPlayerDroppedTreasure = Kill(player, -knResult);
            tileMonster.AddTreasure(knPlayerDroppedTreasure);

            // Replace the current action for that player with an idle action
            CancelAction(static_cast<unsigned>(player.GetID()));
        }
        else
        {
            std::cout << "Joueur " << static_cast<unsigned>(player.GetID()) << " (Score: " << knResult << ")" << std::endl;

            player.AddTreasure(tileMonster.GetTreasureValue());
            tileMonster.Defeat();
        }
    }

    int Game::GameHandler::Fight(unsigned force1, unsigned force2)
    {
        const auto knTotalStrength = force1 + force2;

        std::uniform_int_distribution<int> distCombatRoll(1, knTotalStrength);
        const auto knCombatRoll = distCombatRoll(m_Game.m_numberGenerator);

        if(m_Game.HasDebug())
        {
            std::cout << "Total Strength: " << knTotalStrength << ". Combat roll: " << knCombatRoll << std::endl;
        }

        // If the roll falls within the strength of the force 1, it wins
        // Otherwise, the other wins
        if(knCombatRoll <= static_cast<int>(force1))
        {
            // ex:
            // force 1 = 3, roll = 2 -> ret = 2
            // force 1 = 5, roll = 2 -> ret = 4
            return static_cast<int>(force1) - knCombatRoll + 1;
        }
        else
        {
            // ex:
            // total force = 8, force 2 = 5, roll = 4 -> ret = -1
            // total force = 5, force 2 = 4, roll = 5 -> ret = -4
            // total force = 13, force 2 = 7, roll = 9 -> ret = -3
            return knTotalStrength - (static_cast<int>(force2) + knCombatRoll);
        }
    }

    void Game::GameHandler::CancelAction(size_t nIndex)
    {
        if(m_apChosenActions[nIndex-1])
            m_apChosenActions[nIndex-1].reset(new Action { GetPlayer(nIndex), Action::Direction::Idle });
    }

    unsigned Game::GameHandler::Kill(Player& player, unsigned strength)
    {
        const sf::Vector2i& pos = FindSafestSpawn(GetOtherPlayer(player));

        player.Teleport(pos);
        player.Stun(strength);
        const auto knLostTreasure = player.RemoveTreasure(strength);

        CancelAction(static_cast<unsigned>(player.GetID()));

        return knLostTreasure;
    }

    sf::Vector2i Game::GameHandler::FindSafestSpawn(const Player& enemyPlayer)
    {
        const sf::Vector2i& posSpawn1 = GetRoom().GetStartPositionPlayer1();
        auto dist1 = SquaredNorm(posSpawn1 - enemyPlayer.GetPosition());

        const sf::Vector2i& posSpawn2 = GetRoom().GetStartPositionPlayer2();
        auto dist2 = SquaredNorm(posSpawn2 - enemyPlayer.GetPosition());

        if(dist1 < dist2)
        {
            return posSpawn2;
        }
        else if(dist1 > dist2)
        {
            return posSpawn1;
        }
        else
        {
            return m_Game.GetRandom()%2 ? posSpawn1 : posSpawn2;
        }
    }

    Player& Game::GameHandler::GetPlayer(size_t nIndex) noexcept
    {
        return GetRoom().GetPlayer(nIndex);
    }

    const Player& Game::GameHandler::GetOtherPlayer(const Player& player) const noexcept
    {
        return GetRoom().GetOtherPlayer(player);
    }

    void Game::GameHandler::Render()
    {
        GetRoom().Render(m_Game.m_MainWindow);
    }
}
