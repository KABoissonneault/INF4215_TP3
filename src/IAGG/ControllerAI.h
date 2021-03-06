#pragma once

#include "Controller.h"
#include "TileTreasure.h"
#include "StateAction.h"
#include <map>
#include "AStar.h"

namespace INF4215_TP3
{
    class State;
    class Action;
    struct QNode;
    struct StateAction;
    struct StateActionComparator;

    class ControllerAI : public IController
    {
        public:
            ControllerAI(const Player& player, const Player& other);
            virtual std::unique_ptr<Action> ChooseAction() override;
            QNode GetNode(State state, unsigned target);

            virtual void OnGameEnd(bool hasWon) override;
            virtual void OnStun(unsigned nTurnCount) override;

        protected:
        private:
            const Player& OtherPlayer;
            std::map<StateAction,QNode, StateActionComparator> LearningNodes;
            unsigned alpha;
            State* currentState;
            unsigned currentTarget;
            float currentReward;

            Ketrud::AStar pathfinder;


    };
}

