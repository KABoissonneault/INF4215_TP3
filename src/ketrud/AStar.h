#pragma once

#include "Action.h"
#include "VectorOperations.h"

#include <map>
#include <stack>
#include <memory>
#include <SFML/System/Vector2.hpp>

namespace INF4215_TP3
{
    class Room;
    class Player;

    namespace Ketrud
    {
        struct Node;

        class AStar
        {
        public:
            AStar(const Room&);

            bool FindPath(const sf::Vector2i& source, const sf::Vector2i& destination);
            bool PopDirection(Action::Direction& eDir);

        private:
            const Room& GetRoom()
            {
                return m_Room;
            }

            void FindAndAddNeighbours(Node&);
            std::shared_ptr<Node> GetNode(sf::Vector2i pos);
            double GetNodeHeuristic(const Node&);

            std::map<sf::Vector2i, std::shared_ptr<Node>> m_mapNodes;
            std::stack<Action::Direction> m_pathDirections;

            sf::Vector2i m_Destination;

            const Room& m_Room;

        };
    }
}
