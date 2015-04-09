#pragma once

#include <memory>

#include "Game.h"

namespace INF4215_TP3
{
    class Player;
    class Room;
    class Action;
    class TileMonster;

    class Game::GameHandler
    {
    public:
        GameHandler(Game& game);
        ~GameHandler();

        void Initialize();
        void Update();
        void Render();

        const Player* GetPlayer(size_t nIndex);

    private:
        void ExecuteActions();
        bool CheckIfEngages(const Player&, const Action&);
        int MakePlayersFight();
        int FightPlayerMonster(const Player&, const TileMonster&);
        int Fight(unsigned force1, unsigned force2);
        void Kill(Player&, unsigned strength);
        sf::Vector2i FindSafestSpawn(const Player& enemyPlayer);

        Game& m_Game;
        std::unique_ptr<Room> m_pRoom;
        std::unique_ptr<Player> m_apPlayer[2];
        std::unique_ptr<Player>& m_pPlayer1;
        std::unique_ptr<Player>& m_pPlayer2;
        std::unique_ptr<std::unique_ptr<Action>[]> m_apChosenActions;
    };
}
