#include "ketrud/AStar.h"

#include <queue>

#include "Room.h"
#include "Player.h"
#include "TileTreasure.h"
#include "TileMonster.h"

namespace INF4215_TP3
{
    namespace Ketrud
    {
        struct Node
        {
            Node(const sf::Vector2i& pos)
                : Position(pos)
            {

            }

            const sf::Vector2i Position;
            std::vector<std::pair<std::shared_ptr<Node>, Action::Direction>> Neighbours;

            bool operator<(const Node& other) const noexcept
            {
                return Position < other.Position;
            }
        };

        struct NodeInfo
        {
            NodeInfo(std::shared_ptr<Node> node, std::shared_ptr<NodeInfo> parent, Action::Direction sourceDir, double cost, double heuristic)
                : CurrentNode{node},
                Parent{parent},
                SourceDirection{sourceDir},
                G{cost}, H{heuristic}
            {

            }

            const sf::Vector2i& GetPosition() const
            {
                return CurrentNode->Position;
            }

            std::shared_ptr<Node> CurrentNode;
            std::shared_ptr<NodeInfo> Parent;
            Action::Direction SourceDirection;
            double G;
            double H;

            bool operator>(const NodeInfo& other) const noexcept
            {
                return (G+H) > (other.G+other.H);
            }
        };


        namespace
        {
            struct NodeInfoComparer
            {
                bool operator()(const std::shared_ptr<NodeInfo>& first, const std::shared_ptr<NodeInfo>& second) const noexcept
                {
                    return (*first) > (*second);
                }
            };
        }


        AStar::AStar(const Room& room, const Player& player)
            : m_Room{room},
            m_Player{player}
        {

        }

        std::shared_ptr<Node> AStar::GetNode(const sf::Vector2i pos)
        {
            const auto itFind = m_mapNodes.find(pos);
            if(itFind == end(m_mapNodes))
            {
                auto pNode = std::make_shared<Node>(pos);
                auto retPair = m_mapNodes.insert(std::make_pair(pos, pNode));

                FindAndAddNeighbours(*pNode);

                return retPair.first->second;
            }
            else
            {
                return itFind->second;
            }
        }

        bool AStar::FindPath(const sf::Vector2i& sourcePos, const sf::Vector2i& destinationPos)
        {
            m_Destination = destinationPos;
            while(m_pathDirections.size())
            {
                m_pathDirections.pop();
            }

            if(sourcePos == destinationPos)
            {
                m_pathDirections.push(Action::Direction::Idle);
                return true;
            }

            std::priority_queue<std::shared_ptr<NodeInfo>, std::vector<std::shared_ptr<NodeInfo>>, NodeInfoComparer> frontier;
            std::set<sf::Vector2i> visited;

            auto pSource = GetNode(sourcePos);
            frontier.push(std::make_shared<NodeInfo>(pSource, nullptr, Action::Direction::Idle, 0, GetNodeHeuristic(*pSource)));

            while(frontier.size() != 0)
            {

                auto nodeInfo = frontier.top();
                frontier.pop();

                if(nodeInfo->GetPosition() == m_Destination)
                {
                    m_nPathLength = nodeInfo->G;

                    m_pathDirections.push(nodeInfo->SourceDirection);
                    auto pParent = nodeInfo->Parent;

                    while(pParent && pParent->Parent)
                    {
                        m_pathDirections.push(pParent->SourceDirection);
                        pParent = pParent->Parent;
                    }

                    return true;
                }
                else if(visited.find(nodeInfo->GetPosition()) != end(visited))
                {
                    continue;
                }
                else
                {
                    visited.insert(nodeInfo->GetPosition());

                    for(auto pairNeighbourDir : nodeInfo->CurrentNode->Neighbours)
                    {
                        const auto pNeighbour = pairNeighbourDir.first;
                        const auto eSourceDir = pairNeighbourDir.second;
                        auto fHeuristic = GetNodeHeuristic(*pNeighbour);
                        frontier.push(std::make_shared<NodeInfo>(pNeighbour, nodeInfo, eSourceDir, nodeInfo->G + 1.0, fHeuristic));
                    }
                }
            }

            return false;
        }

        void AStar::FindAndAddNeighbours(Node& node)
        {
            if(node.Neighbours.size() != 0)
            {
                return;
            }

            // On skip la direction Idle
            for(auto it = begin(Action::GetDirections()) + 1; it != end(Action::GetDirections()); ++it)
            {
                const auto eDir = *it;
                const auto posNeighbour = node.Position + eDir;
                const auto pNeighbourTile = GetRoom().GetTile(posNeighbour);
                if(!pNeighbourTile || pNeighbourTile->isSolid())
                    continue;

                auto pNode = GetNode(posNeighbour);
                node.Neighbours.push_back(std::make_pair(pNode, eDir));
            }
        }

        double AStar::GetNodeHeuristic(const Node& node)
        {
            double nHeuristic = Norm(m_Destination - node.Position);

            auto pTile = GetRoom().GetTile(node.Position);
            if(pTile->GetTileType() == TileType::Treasure)
            {
                auto pTreasure = static_cast<const TileTreasure*>(pTile);
                nHeuristic -= pTreasure->GetTreasureValue()*16;
                nHeuristic -= pTreasure->GetWeaponValue()*16;
            }
            else if(pTile->GetTileType() == TileType::Monster)
            {
                auto pMonster = static_cast<const TileMonster*>(pTile);
                nHeuristic += (pMonster->GetStrength() - GetPlayer().GetWeapon() + 2) * 3;
                nHeuristic -= (pMonster->GetTreasureValue());
            }

            return nHeuristic;
        }

        bool AStar::PopDirection(Action::Direction& eDir)
        {
            if(m_pathDirections.size() > 0)
            {
                auto direction = m_pathDirections.top();
                m_pathDirections.pop();
                eDir = direction;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
