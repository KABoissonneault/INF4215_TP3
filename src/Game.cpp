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
    unsigned Game::s_nWindowX = 800;
    unsigned Game::s_nWindowY = 600;

    const unsigned& Game::knWindowX = Game::s_nWindowX;
    const unsigned& Game::knWindowY = Game::s_nWindowY;


    Game& Game::Instance()
    {
        static Game instance;
        return instance;
    }


    Game::Game()
        : m_aLastFrameKeys(knNumKeys, false),
        m_aFrameKeys(knNumKeys, false),
        m_aLastFrameLetters(knNumLetters, false),
        m_aFrameLetters(knNumLetters, false),
        m_numberGenerator(std::chrono::system_clock::now().time_since_epoch().count()),
        m_bInitialized{false},
        m_bPause{false},
        m_nMapSizeX{5},
        m_nMapSizeY{5},
        m_nWantedSeed{knDefaultSeed},
        m_nTrailMaxLength{0},
        m_bDebug{false}
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
            m_MainWindow.create(sf::VideoMode(knWindowX, knWindowY), "INF4215 TP3");
            m_MainWindow.setFramerateLimit(60);

            m_MainFont.loadFromFile("src/ChunkFive.otf");

            {
                m_LoadingText.setString("Chargement de la carte...");
                m_LoadingText.setFont(m_MainFont);
                m_LoadingText.setColor(sf::Color::White);
                const auto& textRect = m_PauseText.getLocalBounds();
                m_LoadingText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
                m_LoadingText.setPosition(m_MainWindow.mapPixelToCoords(sf::Vector2i(knWindowX/2, knWindowY/2)));

                m_MainWindow.clear();

                m_MainWindow.draw(m_LoadingText);

                m_MainWindow.display();
            }

            {
                m_PauseText.setString("Paused");
                m_PauseText.setFont(m_MainFont);
                m_PauseText.setColor(sf::Color::Red);
                const auto& textRect = m_PauseText.getLocalBounds();
                m_PauseText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
                m_PauseText.setPosition(m_MainWindow.mapPixelToCoords(sf::Vector2i(knWindowX/2, knWindowY/2)));
            }

            // Game creation
            m_pGameHandler->Initialize();

            m_bInitialized = true;

            std::cout << std::endl;
        }
    }

    void Game::ParseArgs(const std::vector<std::string>& args)
    {
        try
        {
            for(size_t i = 1; i < args.size(); ++i)
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
                else if(arg == "max_trail")
                {
                    const std::string& sLength = args.at(i+1);

                    auto nResult = sscanf( sLength.c_str(), "%u", &m_nTrailMaxLength);
                    if(nResult != 1)
                    {
                        throw std::runtime_error("Le premier argument suivant \"max_trail\" n'etait pas un nombre");
                    }

                    std::cout << "La taille maximale des trails sera " << m_nTrailMaxLength << std::endl;

                    ++i;
                }
                else if(arg == "window_size")
                {
                    const std::string& sSizeX = args.at(i+1);
                    const std::string& sSizeY = args.at(i+2);

                    auto nResult = sscanf( sSizeX.c_str(), "%u", &s_nWindowX);
                    if(nResult != 1)
                    {
                        throw std::runtime_error("Le premier argument suivant \"window_size\" n'etait pas un nombre");
                    }
                    nResult = sscanf( sSizeY.c_str(), "%u", &s_nWindowY);
                    if(nResult != 1)
                    {
                        throw std::runtime_error("Le deuxieme argument suivant \"window_size\" n'etait pas un nombre");
                    }

                    std::cout << "La fenetre cree sera de taille " << s_nWindowX << "x" << s_nWindowY << std::endl;

                    i += 2;
                }
                else if(arg == "delay")
                {
                    const std::string& sDelay = args.at(i+1);

                    auto nResult = sscanf( sDelay.c_str(), "%u", &m_nMSMinimumDelay);
                    if(nResult != 1)
                    {
                        throw std::runtime_error("Le premier argument suivant \"delay\" n'etait pas un nombre");
                    }

                    std::cout << "Le delais minimal entre 2 tours sera " << m_nMSMinimumDelay << std::endl;

                    ++i;
                }
                else if(arg == "debug")
                {
                    m_bDebug = true;
                }
                else
                {
                    std::cout << "Argument invalide \"" << arg << "\"" << std::endl;
                }
            }
        }
        catch(std::exception& e)
        {
            std::cout << "Erreur dans l'intepretation des arguments: " << e.what() << std::endl;
        }

        std::cout << std::endl;
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
        for(unsigned i = 0; i < knNumLetters; ++i)
        {
            m_aLastFrameLetters[i] = m_aFrameLetters[i];
        }
    }

    void Game::Run()
    {
        assert(m_bInitialized && "Programme non initialize avant l'execution");

        std::cout << "Debut de la partie!" << std::endl;

        //m_timeNextUpdateMinimum = std::chrono::high_resolution_clock::now();

        while(m_MainWindow.isOpen())
        {
            PollEvents();
            if(!m_bPause /*&& std::chrono::high_resolution_clock::now() >= m_timeNextUpdateMinimum*/)
            {
                Update();
                //m_timeNextUpdateMinimum = std::chrono::steady_clock::now() + std::chrono::milliseconds(50);
            }

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
                    case sf::Keyboard::Key::Numpad5:
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
                    if(event.key.code >= sf::Keyboard::Key::A && event.key.code <= sf::Keyboard::Key::Z)
                    {
                        unsigned index = event.key.code - sf::Keyboard::Key::A;
                        m_aFrameLetters[index] = true;
                    }


                }
                break;

            case sf::Event::KeyReleased:
                {
                    switch(event.key.code)
                    {
                    case sf::Keyboard::Key::Space:
                        m_bPause = !m_bPause;
                        break;

                    case sf::Keyboard::Key::Numpad1:
                    case sf::Keyboard::Key::Numpad2:
                    case sf::Keyboard::Key::Numpad3:
                    case sf::Keyboard::Key::Numpad4:
                    case sf::Keyboard::Key::Numpad5:
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
                    if(event.key.code >= sf::Keyboard::Key::A && event.key.code <= sf::Keyboard::Key::Z)
                    {
                        unsigned index = event.key.code - sf::Keyboard::Key::A;
                        m_aFrameLetters[index] = false;
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

        if(m_bPause)
        {
            m_MainWindow.draw(m_PauseText);
        }

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
