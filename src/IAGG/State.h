#pragma once

#include <vector>
#include "Action.h"
#include "StateNode.h"

namespace INF4215_TP3
{
    class StateNode;
    class Player;
    class ITile;

    class State
    {
        public:
            State(const Player& player);
            virtual ~State();

            bool operator==(const State lhs) const;

            unsigned Count()
            {
                return listNodes.size();
            }
            sf::Vector2i GetPosition(unsigned index)
            {
                return listNodes[index].GetPosition();
            }

        protected:
            void InitializeState(const Player& player);
            void CalculatePath(StateNode* node, const ITile* tile);

        private:
            Action::Direction DirectionFinding(const sf::Vector2i pos);
            std::vector<StateNode> listNodes;
            unsigned treasure;
            unsigned strength;
            const Player& m_player;
    };
}

