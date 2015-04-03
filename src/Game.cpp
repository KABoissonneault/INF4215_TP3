#include "Game.h"

#include "Room.h"

#include <iostream>
#include <cstdio>

namespace INF4215_TP3
{
    Game& Game::Instance()
    {
        static Game instance;
        return instance;
    }


    Game::Game()
        : m_MainWindow(sf::VideoMode(800, 600), "INF4215 TP3"),
        m_pRoom(nullptr)
    {
        Initialize();
    }

    Game::~Game()
    {
        Clean();
    }

    void Game::Initialize()
    {

        m_MainWindow.setFramerateLimit(60);

        std::cout << "Bienvenue dans notre TP3 de INF4215" << std::endl;

        unsigned x, y;
        std::cout << "Veuillez entrer la taille de la piece initiale comme ceci \"X, Y\": ";
        auto nCount = scanf("%u, %u", &x, &y);
        while(nCount != 2)
        {
            std::cout << std::endl << "Format invalide." << std::endl << "Veuillez entrer la taille de la pièce initiale comme ceci \"X, Y\": ";
            nCount = scanf("%u, %u", &x, &y);
        }
        std::cout << std::endl;

        m_pRoom.reset(new Room(x, y));

        m_pRoom->GenerateRoom(m_MainWindow);
    }

    void Game::Clean()
    {

    }

    void Game::Run()
    {
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
            if (event.type == sf::Event::Closed)
                m_MainWindow.close();
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
