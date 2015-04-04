#include "Game.h"

#include "Room.h"
#include "Tile.h"
#include "Player.h"

#include <iostream>
#include <cstdio>
#include <exception>
#include <chrono>

#include <cassert>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

namespace INF4215_TP3
{
    Game& Game::Instance()
    {
        static Game instance;
        return instance;
    }


    Game::Game()
        : m_MainWindow{sf::VideoMode(knWindowX, knWindowY), "INF4215 TP3"},
        m_pRoom{nullptr},
        m_numberGenerator(std::chrono::system_clock::now().time_since_epoch().count()),
        m_bInitialized{false},
        m_nMapSizeX{5},
        m_nMapSizeY{5},
        m_nWantedSeed{knDefaultSeed}
    {
    }

    Game::~Game()
    {
        Clean();
    }

    void Game::Initialize(const std::vector<std::string>& args)
    {
        if(!m_bInitialized)
        {
            // Parse program args
            ParseArgs(args);

            // Window settings
            m_MainWindow.setFramerateLimit(60);

            // Room creation
            m_pRoom.reset(new Room(m_MainWindow, m_nMapSizeX, m_nMapSizeY));

            if(m_nWantedSeed == knDefaultSeed)
            {
                size_t nChosenSeed;
                do
                {
                    nChosenSeed = GetRandom();
                    m_pRoom->GenerateRoom(nChosenSeed);

                } while (!m_pRoom->ValidateRoom());

                std::cout << "La salle fut generee avec le seed: " << nChosenSeed << std::endl;
            }
            else
            {
                m_pRoom->GenerateRoom(m_nWantedSeed);
                Render();
                if(!m_pRoom->ValidateRoom())
                {
                    throw InitializeException("Le seed passe en argument ne donne pas une salle valide");
                }
            }

            // Player creation
            // Generate a player position until you spawn them on a non-solid tile
            std::uniform_int_distribution<size_t> distributionPos(0, m_pRoom->TileCount() - 1);
            size_t nPosPlayer1 = distributionPos(m_numberGenerator);
            while(m_pRoom->GetTile(nPosPlayer1)->isSolid())
            {
                nPosPlayer1 = distributionPos(m_numberGenerator);
            }
            sf::Vector2i posPlayer1(nPosPlayer1/m_pRoom->GetSizeY(), nPosPlayer1%m_pRoom->GetSizeY());

            size_t nPosPlayer2 = distributionPos(m_numberGenerator);
            while(m_pRoom->GetTile(nPosPlayer2)->isSolid() || nPosPlayer2 == nPosPlayer1)
            {
                nPosPlayer2 = distributionPos(m_numberGenerator);
            }
            sf::Vector2i posPlayer2(nPosPlayer2/m_pRoom->GetSizeY(), nPosPlayer2%m_pRoom->GetSizeY());

            m_pPlayer1.reset( new Player(*m_pRoom, Player::ID::Player1, Player::ControllerType::Input, posPlayer1) );
            m_pPlayer2.reset( new Player(*m_pRoom, Player::ID::Player2, Player::ControllerType::Input, posPlayer2) );

            m_bInitialized = true;
        }
    }

    void Game::ParseArgs(const std::vector<std::string>& args)
    {
        std::cout << std::endl;

        try
        {
            for(size_t i = 0; i < args.size(); ++i)
            {
                const std::string& arg = args.at(i);

                if(arg == "size")
                {
                    const std::string& sSizeX = args.at(i+1);
                    const std::string& sSizeY = args.at(i+2);

                    auto nResult = sscanf( sSizeX.c_str(), "%u", &m_nMapSizeX);
                    if(nResult != 1)
                    {
                        throw std::runtime_error("Le premier argument suivant \"size\" n'etait pas un nombre");
                    }
                    nResult = sscanf( sSizeY.c_str(), "%u", &m_nMapSizeY);
                    if(nResult != 1)
                    {
                        throw std::runtime_error("Le deuxieme argument suivant \"size\" n'etait pas un nombre");
                    }

                    std::cout << "La salle cree sera de taille " << m_nMapSizeX << "x" << m_nMapSizeY << std::endl;

                    i += 2;
                }
                else if (arg == "seed")
                {
                    const std::string& seed = args.at(i+1);

                    auto nResult = sscanf( seed.c_str(), "%u", &m_nWantedSeed);
                    if(nResult != 1)
                    {
                        throw std::runtime_error("Le premier argument suivant \"seed\" n'etait pas un nombre");
                    }

                    std::cout << "Le seed de la salle cree sera " << m_nWantedSeed << std::endl;

                    ++i;
                }
            }
        }
        catch(std::exception& e)
        {
            std::cout << "Erreur dans l'intepretation des arguments: " << e.what() << std::endl;
        }
    }

    void Game::Clean()
    {

    }

    void Game::Run()
    {
        assert(m_bInitialized && "Programme non initialize avant l'execution");

        Render();

        while(m_MainWindow.isOpen())
        {
            PollEvents();
            Update();
            Render();
        }
    }

    void Game::PollEvents()
    {
        sf::Event event;
        while (m_MainWindow.pollEvent(event))
        {
            // Close window : exit
            switch(event.type)
            {
            case sf::Event::Closed:
                m_MainWindow.close();
                break;

            case sf::Event::KeyPressed:
                {
                    switch(event.key.code)
                    {
                    case sf::Keyboard::Key::Escape:
                        m_MainWindow.close();
                        break;

                    default:
                        break;
                    }

                }
                break;

            default:
                break;
            }
        }
    }

    void Game::Update()
    {

    }

    void Game::Render()
    {
        m_MainWindow.clear();

        m_pRoom->Render(m_MainWindow);

        // Draw players
        m_MainWindow.draw(m_pPlayer1->GetSprite(), m_pRoom->getTransform());
        m_MainWindow.draw(m_pPlayer2->GetSprite(), m_pRoom->getTransform());

        m_MainWindow.display();
    }



    Game::InitializeException::InitializeException(const std::string& s)
        : m_sText(s)
    {

    }
    const char* Game::InitializeException::what() const noexcept
    {
        return m_sText.c_str();
    }

}

#pragma GCC diagnostic pop
