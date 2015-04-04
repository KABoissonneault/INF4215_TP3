#include "Game.h"

#include "Room.h"

#include <iostream>
#include <cstdio>
#include <exception>

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
        m_bInitialized{false},
        m_nMapSizeX{5},
        m_nMapSizeY{5}
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

            m_pRoom.reset(new Room(m_nMapSizeX, m_nMapSizeY));

            m_pRoom->GenerateRoom();

            m_bInitialized = true;
        }
    }

    void Game::ParseArgs(const std::vector<std::string>& args)
    {
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
                        throw std::runtime_error("First argument following \"size\" was not an unsigned number");
                    }
                    nResult = sscanf( sSizeY.c_str(), "%u", &m_nMapSizeY);
                    if(nResult != 1)
                    {
                        throw std::runtime_error("First argument following \"size\" was not an unsigned number");
                    }

                    i += 2;
                }
            }
        }
        catch(std::exception& e)
        {
            std::cout << "Error in args parsing, using default values: " << e.what() << std::endl;
        }
    }

    void Game::Clean()
    {

    }

    void Game::Run()
    {
        if(!m_bInitialized)
            throw std::runtime_error("Program was not initialized before running");

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
