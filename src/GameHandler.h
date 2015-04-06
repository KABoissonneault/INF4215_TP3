#pragma once

#include <memory>

#include "Game.h"

namespace INF4215_TP3
{
    class Player;
    class Room;
    class Action;

    class Game::GameHandler
    {
    public:
        GameHandler(Game& game);
        ~GameHandler();

        void Initialize();
        void Update();
        void Render();

    private:
        void ExecuteActions();

        Game& m_Game;
        std::unique_ptr<Room> m_pRoom;
        std::unique_ptr<Player> m_pPlayer1;
        std::unique_ptr<Player> m_pPlayer2;
        std::unique_ptr<std::unique_ptr<Action>[]> m_apChosenActions;
    };
}
