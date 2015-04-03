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

        sf::RenderWindow m_MainWindow;
        std::unique_ptr<Room> m_pRoom;

        void Initialize();
        void Clean();

        void PollEvents();
        void Update();
        void Render();
    };
}
