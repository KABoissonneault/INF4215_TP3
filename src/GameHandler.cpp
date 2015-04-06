#include "GameHandler.h"

#include "Player.h"
#include "Room.h"
#include "Tile.h"
#include "Action.h"

#include <iostream>

namespace INF4215_TP3
{
    Game::GameHandler::GameHandler(Game& game)
        : m_Game(game),
        m_apChosenActions{new std::unique_ptr<Action>[2]}
    {

    }
    Game::GameHandler::~GameHandler() = default;


    void Game::GameHandler::Initialize()
    {
        m_pRoom.reset(new Room(m_Game.m_MainWindow, m_Game.m_nMapSizeX, m_Game.m_nMapSizeY));

        if(m_Game.m_nWantedSeed == Game::knDefaultSeed)
        {
            size_t nChosenSeed;
            do
            {
                nChosenSeed = m_Game.GetRandom();
                m_pRoom->GenerateRoom(nChosenSeed);

            } while (!m_pRoom->ValidateRoom());

            std::cout << "La salle fut generee avec le seed: " << nChosenSeed << std::endl;
        }
        else
        {
            m_pRoom->GenerateRoom(m_Game.m_nWantedSeed);
            if(!m_pRoom->ValidateRoom())
            {
                throw InitializeException("Le seed passe en argument ne donne pas une salle valide");
            }
        }

        // Player creation
        // Generate a player position until you spawn them on a non-solid tile
        std::uniform_int_distribution<size_t> distributionPos(0, m_pRoom->TileCount() - 1);
        size_t nPosPlayer1 = distributionPos(m_Game.m_numberGenerator);
        while(m_pRoom->GetTile(nPosPlayer1)->isSolid())
        {
            nPosPlayer1 = distributionPos(m_Game.m_numberGenerator);
        }
        sf::Vector2i posPlayer1(nPosPlayer1/m_pRoom->GetSizeY(), nPosPlayer1%m_pRoom->GetSizeY());

        size_t nPosPlayer2 = distributionPos(m_Game.m_numberGenerator);
        while(m_pRoom->GetTile(nPosPlayer2)->isSolid() || nPosPlayer2 == nPosPlayer1)
        {
            nPosPlayer2 = distributionPos(m_Game.m_numberGenerator);
        }
        sf::Vector2i posPlayer2(nPosPlayer2/m_pRoom->GetSizeY(), nPosPlayer2%m_pRoom->GetSizeY());

        m_pPlayer1.reset( new Player(*m_pRoom, Player::ID::Player1, Player::ControllerType::Input, posPlayer1) );
        m_pPlayer2.reset( new Player(*m_pRoom, Player::ID::Player2, Player::ControllerType::Input, posPlayer2) );

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
        m_pPlayer1->ExecuteAction(*m_apChosenActions[0]);
        m_pPlayer2->ExecuteAction(*m_apChosenActions[1]);

        m_apChosenActions[0].reset();
        m_apChosenActions[1].reset();
    }

    void Game::GameHandler::Render()
    {
        m_pRoom->Render(m_Game.m_MainWindow);

        // Draw players
        m_Game.m_MainWindow.draw(m_pPlayer1->GetSprite(), m_pRoom->getTransform());
        m_Game.m_MainWindow.draw(m_pPlayer2->GetSprite(), m_pRoom->getTransform());
    }
}
