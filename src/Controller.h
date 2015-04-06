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

        virtual std::unique_ptr<Action> ChooseAction() = 0;

        //virtual void OnStun() = 0;

    protected:
        const Player& m_Player;
    };
}
