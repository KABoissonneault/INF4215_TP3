#pragma once

#include <SFML/Graphics.hpp>

#include <exception>
#include <memory>

namespace INF4215_TP3
{
    class Room;

    class Game
    {
    public:
        static Game& Instance();
        static const unsigned knWindowX = 800;
        static const unsigned knWindowY = 600;

        void Initialize(const std::vector<std::string>& args);

        void Run();

        class InitializeException : public std::exception
        {
        public:
            InitializeException(const std::string&);

            virtual const char* what() const noexcept override;

        private:
            std::string m_sText;
        };

        const sf::RenderWindow& GetWindow() const
        {
            return m_MainWindow;
        }

    private:
        Game();
        ~Game();

        void ParseArgs(const std::vector<std::string>& args);

        void Clean();

        void PollEvents();
        void Update();
        void Render();

        sf::RenderWindow m_MainWindow;
        std::unique_ptr<Room> m_pRoom;

        bool m_bInitialized;

        // Args results
        unsigned m_nMapSizeX;
        unsigned m_nMapSizeY;
    };
}
