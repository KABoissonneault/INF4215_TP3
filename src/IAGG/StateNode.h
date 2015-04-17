#pragma once

#include "Tile.h"
#include "Action.h"

namespace INF4215_TP3
{
    class TileTreasure;
    class TileMonster;
    class Player;

    enum NodeType{
        NodeTreasure,
        NodeMonster,
        NodePlayer
    };

    class StateNode
    {
        public:
            StateNode();
            virtual ~StateNode();
            StateNode(const TileTreasure* tile);
            StateNode(const TileMonster* tile);
            StateNode(const Player* player);

            unsigned GetTreasure();
            unsigned GetWeapon();
            Action::Direction GetDirection();
            unsigned GetDistance();
            NodeType GetType();
            sf::Vector2i GetPosition();

            void SetDirection(Action::Direction direc);
            void SetDistance(unsigned dist);

            bool operator<(const StateNode& rhs) const;
            bool operator==(const StateNode& rhs) const;

        protected:
        private:
            unsigned distance;
            Action::Direction direction;
            NodeType type;
            unsigned weapon;
            unsigned treasure;
            sf::Vector2i position;
    };

    bool operator<(const StateNode& lhs, const StateNode& rhs);
}

