#include "StateNode.h"

#include "Game.h"
#include "Player.h"
#include "Action.h"
#include "Room.h"
#include "TileTreasure.h"
#include "TileMonster.h"


namespace INF4215_TP3
{
    StateNode::StateNode()
    {
        //ctor
    }

    StateNode::~StateNode()
    {
        //dtor
    }


    StateNode::StateNode(const TileTreasure* tile)
    {
        treasure = tile->GetTreasureValue();
        weapon = tile->GetWeaponValue();
        type = NodeType::NodeTreasure;
        position = tile->GetPosition();
    }

    StateNode::StateNode(const TileMonster* tile)
    {
        treasure = tile->GetTreasureValue();
        weapon = tile->GetStrength();
        type = NodeType::NodeMonster;
        position = tile->GetPosition();
    }

    StateNode::StateNode(const Player* player)
    {
        treasure = player->GetTreasure();
        weapon = player->GetWeapon();
        type = NodeType::NodePlayer;
        position = player->GetPosition();
    }

    unsigned StateNode::GetTreasure()
    {
        return treasure;
    }

    unsigned StateNode::GetWeapon()
    {
        return weapon;
    }

    unsigned StateNode::GetDistance()
    {
        return distance;
    }

    Action::Direction StateNode::GetDirection()
    {
        return direction;
    }

    NodeType StateNode::GetType()
    {
        return type;
    }

    void StateNode::SetDirection(Action::Direction direc)
    {
        direction = direc;
    }

    void StateNode::SetDistance(unsigned dist)
    {
        distance = dist;
    }

    bool StateNode::operator<(const StateNode& rhs) const
    {
        return this->distance < rhs.distance;
    }

    bool StateNode::operator==(const StateNode& rhs) const
    {
        return this->distance == rhs.distance;
    }

    sf::Vector2i StateNode::GetPosition()
    {
         return position;
    }
}
