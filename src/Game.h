#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <exception>
#include <memory>
#include <random>
//#include <chrono>

namespace INF4215_TP3
{
    class Room;
    class Player;

    class Game
    {
    public:
        static Game& Instance();

    public:
        static const unsigned& knWindowX;
        static const unsigned& knWindowY;


        void Initialize(const std::vector<std::string>& args);

        void Run();

        // La valeur du numpad (donc 1-9)
        static const unsigned knNumKeys = 10;
        bool NumpadWasPressed(unsigned key)
        {
            return !m_aLastFrameKeys[key] && m_aFrameKeys[key];
        }

        static const unsigned knNumLetters = 26;
        bool LetterWasPressed(char letter)
        {
            unsigned index = letter - 'a';
            return !m_aLastFrameLetters[index] && m_aFrameLetters[index];
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

        unsigned GetMaxTrail() const
        {
            return m_nTrailMaxLength;
        }

        bool HasDebug() const
        {
            return m_bDebug;
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
        sf::Font m_MainFont;
        sf::Text m_PauseText;
        sf::Text m_LoadingText;
        sf::Text m_VictoryText;

        //std::chrono::high_resolution_clock::time_point m_timeNextUpdateMinimum;

        // Takes care of the game flow
        // Unlike Game itself which handle the "app" flow
        std::unique_ptr<GameHandler> m_pGameHandler;

        std::vector<bool> m_aLastFrameKeys;
        std::vector<bool> m_aFrameKeys;
        std::vector<bool> m_aLastFrameLetters;
        std::vector<bool> m_aFrameLetters;

        std::default_random_engine m_numberGenerator;
        bool m_bInitialized;
        bool m_bPause;
        bool m_bWon;

        // Args results
        unsigned m_nMapSizeX;
        unsigned m_nMapSizeY;
        unsigned m_nWantedSeed;
        unsigned m_nTrailMaxLength;
        unsigned m_nMSMinimumDelay;
        bool m_bDebug;

        static unsigned s_nWindowX;
        static unsigned s_nWindowY;

        static const unsigned knDefaultSeed = 0;
    };
}
