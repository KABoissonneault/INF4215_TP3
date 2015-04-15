#pragma once

#include <map>

#include "Controller.h"
#include "Action.h"

namespace INF4215_TP3
{
    class Player;
    class Action;

    namespace Ketrud
    {
        class StateGame;

        struct StateAction
        {
            std::shared_ptr<const StateGame> State;
            Action ChosenAction;

            bool operator==(const StateAction&) const;
        };

        class ControllerAI final : public IController
        {
        public:
            ControllerAI(const Player&);
            ~ControllerAI();

            virtual std::unique_ptr<Action> ChooseAction() override;

            virtual void OnStun(unsigned nTurnCount) override;
            virtual void OnTreasureChange(int nTreasureChange) override;
            virtual void OnTurnEnd() override;

        private:
            std::map<StateAction, int> m_mapQValue;
            std::shared_ptr<const StateGame> m_pCurrentState;
            int nCurrentReward;

        };
    }
}

namespace std
{
    template<>
    struct hash<INF4215_TP3::Ketrud::StateAction>
    {
        size_t operator()(const INF4215_TP3::Ketrud::StateAction&) const;
    };
}
