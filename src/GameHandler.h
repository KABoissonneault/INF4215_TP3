#pragma once

#include "Game.h"

#include <memory>
#include <cassert>

namespace INF4215_TP3
{
    class Player;
    class Room;
    class Action;
    class TileMonster;

    class Game::GameHandler
    {
    public:
        static const size_t knPlayerCount = 2;
        GameHandler(Game& game);
        ~GameHandler();

        void Initialize();
        void Update();
        void Render();

        // GameHandler must be initialized first
        // nIndex must be smaller than knPlayerCount
        Player& GetPlayer(size_t nIndex) noexcept
        {
            assert(m_bInitialized && nIndex <= knPlayerCount && nIndex > 0);

            return *m_apPlayer[nIndex - 1];
        }

        const Player& GetPlayer(size_t nIndex) const noexcept
        {
            return const_cast<GameHandler* const>(this)->GetPlayer(nIndex);
        }

        const Player& GetOtherPlayer(const Player& player) const noexcept;

        const Room& GetRoom() const noexcept
        {
            return *m_pRoom;
        }


    private:


        Action& GetAction(size_t nIndex)
        {
            assert(nIndex <= knPlayerCount && nIndex > 0 && m_bInitialized);

            return *m_apChosenActions[nIndex - 1];
        }

        void ExecuteActions();
        void CheckMonsterEngagements();
        void CheckPlayerEngagements();
        bool CheckIfFirstEngagesSecond(const sf::Vector2i& movementSource, const sf::Vector2i& movementTarget);
        void MakePlayersFight();
        void FightPlayerMonster(Player&, TileMonster&);
        int Fight(unsigned force1, unsigned force2);
        // Returns the treasure that was removed from the player
        void CancelAction(size_t nIndex);
        unsigned Kill(Player&, unsigned strength);
        sf::Vector2i FindSafestSpawn(const Player& enemyPlayer);

        Game& m_Game;
        std::unique_ptr<Room> m_pRoom;
        std::unique_ptr<Player> m_apPlayer[2];
        std::unique_ptr<Player>& m_pPlayer1;
        std::unique_ptr<Player>& m_pPlayer2;
        std::unique_ptr<std::unique_ptr<Action>[]> m_apChosenActions;

        bool m_bInitialized;
    };
}
