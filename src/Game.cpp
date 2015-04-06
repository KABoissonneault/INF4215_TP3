#include "Game.h"

#include "Room.h"
#include "Tile.h"
#include "Player.h"
#include "GameHandler.h"

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
        m_aLastFrameKeys(knNumKeys, false),
        m_aFrameKeys(knNumKeys, false),
        m_numberGenerator(std::chrono::system_clock::now().time_since_epoch().count()),
        m_bInitialized{false},
        m_nMapSizeX{5},
        m_nMapSizeY{5},
        m_nWantedSeed{knDefaultSeed}
    {
        m_pGameHandler.reset(new GameHandler(*this));
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

            // Game creation
            m_pGameHandler->Initialize();

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

    void Game::InitializeKeys()
    {
        for(unsigned i = 0; i < knNumKeys; ++i)
        {
            m_aLastFrameKeys[i] = m_aFrameKeys[i];
        }
    }

    void Game::Run()
    {
        assert(m_bInitialized && "Programme non initialize avant l'execution");

        while(m_MainWindow.isOpen())
        {
            PollEvents();
            Update();
            Render();
        }
    }

    void Game::PollEvents()
    {
        InitializeKeys();
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

                    case sf::Keyboard::Key::Numpad1:
                    case sf::Keyboard::Key::Numpad2:
                    case sf::Keyboard::Key::Numpad3:
                    case sf::Keyboard::Key::Numpad4:
                    case sf::Keyboard::Key::Numpad6:
                    case sf::Keyboard::Key::Numpad7:
                    case sf::Keyboard::Key::Numpad8:
                    case sf::Keyboard::Key::Numpad9:
                        {
                            unsigned keyID = event.key.code - sf::Keyboard::Key::Numpad0;
                            m_aFrameKeys[keyID] = true;
                        }
                        break;


                    default:
                        break;
                    }

                }
                break;

            case sf::Event::KeyReleased:
                {
                    switch(event.key.code)
                    {

                    case sf::Keyboard::Key::Numpad1:
                    case sf::Keyboard::Key::Numpad2:
                    case sf::Keyboard::Key::Numpad3:
                    case sf::Keyboard::Key::Numpad4:
                    case sf::Keyboard::Key::Numpad6:
                    case sf::Keyboard::Key::Numpad7:
                    case sf::Keyboard::Key::Numpad8:
                    case sf::Keyboard::Key::Numpad9:
                        {
                            unsigned keyID = event.key.code - sf::Keyboard::Key::Numpad0;
                            m_aFrameKeys[keyID] = false;
                        }
                        break;


                    default:
                        break;
                    }
                }
            default:
                break;
            }
        }
    }

    void Game::Update()
    {
        m_pGameHandler->Update();
    }

    void Game::Render()
    {
        m_MainWindow.clear();

        m_pGameHandler->Render();

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
