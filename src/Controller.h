#pragma once

#include <memory>

namespace INF4215_TP3
{
    class Action;
    class Player;

    class IController
    {
    public:
        IController(const Player& player)
            : m_Player(player)
        {

        }

        virtual ~IController() = default;

        const Player& GetPlayer() const
        {
            return m_Player;
        }

        virtual std::unique_ptr<Action> ChooseAction() = 0;

        virtual void OnStun(unsigned nTurnCount){ (void) nTurnCount; }
        virtual void OnTreasureChange(int nTreasureChange){ (void) nTreasureChange; }
        virtual void OnTurnEnd(){}
        virtual void OnGameEnd(bool hasWon){ (void) hasWon;}

    protected:
        const Player& m_Player;
    };
}
