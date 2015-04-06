#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <exception>
#include <memory>
#include <random>

namespace INF4215_TP3
{
    class Room;
    class Player;

    class Game
    {
    public:
        static Game& Instance();
        static const unsigned knWindowX = 800;
        static const unsigned knWindowY = 600;

        void Initialize(const std::vector<std::string>& args);

        void Run();

        // La valeur du numpad (donc 1-9)
        static const unsigned knNumKeys = 10;
        bool NumpadWasPressed(unsigned key)
        {
            return !m_aLastFrameKeys[key] && m_aFrameKeys[key];
        }

        std::default_random_engine::result_type GetRandom() const
        {
            return const_cast<Game* const>(this)->m_numberGenerator();
        }

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
        class GameHandler;

        Game();
        ~Game();

        void ParseArgs(const std::vector<std::string>& args);

        void Clean();
        void InitializeKeys();

        void PollEvents();
        void Update();
        void Render();

        sf::RenderWindow m_MainWindow;

        // Takes care of the game flow
        // Unlike Game itself which handle the "app" flow
        std::unique_ptr<GameHandler> m_pGameHandler;

        std::vector<bool> m_aLastFrameKeys;
        std::vector<bool> m_aFrameKeys;

        std::default_random_engine m_numberGenerator;
        bool m_bInitialized;

        // Args results
        unsigned m_nMapSizeX;
        unsigned m_nMapSizeY;
        unsigned m_nWantedSeed;
        static const unsigned knDefaultSeed = 0;
    };
}
