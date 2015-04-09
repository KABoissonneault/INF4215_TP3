#include "GameHandler.h"

#include "Player.h"
#include "Room.h"
#include "Tile.h"
#include "TileMonster.h"
#include "Action.h"

#include <iostream>
#include <cassert>
#include <random>

namespace INF4215_TP3
{
    Game::GameHandler::GameHandler(Game& game)
        : m_Game(game),
        m_pPlayer1(m_apPlayer[0]),
        m_pPlayer2(m_apPlayer[1]),
        m_apChosenActions{new std::unique_ptr<Action>[2]}
    {

    }
    Game::GameHandler::~GameHandler() = default;

     const Player* Game::GameHandler::GetPlayer(size_t nIndex)
     {
        assert(nIndex < 2);
        return m_apPlayer[nIndex].get();
    }

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

        // Player creation
        m_pPlayer1.reset( new Player(*m_pRoom, Player::ID::Player1, Player::ControllerType::Input, m_pRoom->GetStartPositionPlayer1()) );
        m_pPlayer2.reset( new Player(*m_pRoom, Player::ID::Player2, Player::ControllerType::Input, m_pRoom->GetStartPositionPlayer2()) );

    }

    void Game::GameHandler::Update()
    {
        if(!m_apChosenActions[0])
            m_apChosenActions[0] = m_pPlayer1->ChooseAction();
        if(!m_apChosenActions[1])
            m_apChosenActions[1] = m_pPlayer2->ChooseAction();

        // Si on a choisi deux actions, on va les exécuter
        // On fait cela car le ControllerInput retourne souvent des actions nulles
        if(m_apChosenActions[0] && m_apChosenActions[1])
        {
            ExecuteActions();
        }
    }

    void Game::GameHandler::ExecuteActions()
    {
        // Check if the players' action brings them on a monster tile
        for (unsigned i = 0; i < 2; ++i)
        {
            auto& pPlayer = m_apPlayer[i];
            auto& pAction = m_apChosenActions[i];

            const auto& resultingPos = pAction->GetResultingPosition(pPlayer->GetPosition());
            const auto pResultingTile = m_pRoom->GetTile(resultingPos);
            if(pResultingTile->GetTileType() == TileType::Monster)
            {
                const auto knResult = FightPlayerMonster(*pPlayer, *static_cast<TileMonster*>(pResultingTile));
                if(knResult < 0)
                {

                }
            }
        }

        m_pPlayer1->ExecuteAction(*m_apChosenActions[0]);
        m_pPlayer2->ExecuteAction(*m_apChosenActions[1]);

        // Execute the tile's action
        m_pRoom->GetTile(m_pPlayer1->GetPosition())->OnEnter(*m_pPlayer1);
        m_pRoom->GetTile(m_pPlayer2->GetPosition())->OnEnter(*m_pPlayer2);

        m_apChosenActions[0].reset();
        m_apChosenActions[1].reset();
    }

    int Game::GameHandler::FightPlayerMonster(const Player& player, const TileMonster& tileMonster)
    {
        return Fight(player.GetWeapon(), tileMonster.GetStrength());
    }

    int Game::GameHandler::Fight(unsigned force1, unsigned force2)
    {
        const auto knTotalStrength = force1 + force2;

        std::uniform_int_distribution<int> distCombatRoll(1, knTotalStrength);
        const auto knCombatRoll = distCombatRoll(m_Game.m_numberGenerator);

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

    void Game::GameHandler::Kill(Player& player, unsigned strength)
    {
        (void)player;
        (void)strength;
    }

    sf::Vector2i Game::GameHandler::FindSafestSpawn(const Player& enemyPlayer)
    {
        (void)enemyPlayer;
        return sf::Vector2i{};
    }

    void Game::GameHandler::Render()
    {
        m_pRoom->Render(m_Game.m_MainWindow);

        // Draw players
        m_pPlayer1->Render(m_Game.m_MainWindow, m_pRoom->getTransform());
        m_pPlayer2->Render(m_Game.m_MainWindow, m_pRoom->getTransform());
    }
}
