#pragma once

#include "Controller.h"
#include "TileTreasure.h"
#include "StateAction.h"
#include <map>

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
            float GetQ(State, Action);
            float GetA(State,Action);
            float GetN(State,Action);

            void SetQ(State, Action, float);
            void SetA(State,Action, float);
            void SetN(State,Action, float);
        protected:
        private:
            unsigned CompareStrength();
            const TileTreasure* NearestTreasure();
            unsigned PathFinding(ITile* tile);
            const Player& OtherPlayer;
            std::map<StateAction,QNode, StateActionComparator> LearningNodes;
            unsigned alpha;
            //State currentState;

            const float epsilon = 0.2f;
    };
}

