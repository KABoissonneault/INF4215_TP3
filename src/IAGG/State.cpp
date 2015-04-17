#include "State.h"
#include "StateNode.h"
#include "Player.h"
#include "Room.h"
#include "TileTreasure.h"
#include "TileMonster.h"
#include "Tile.h"
#include "Action.h"
#include "ketrud/AStar.h"

namespace INF4215_TP3
{
    State::State(const Player& player)
    : m_player{player}
    {
        treasure = player.GetTreasure();
        strength = player.GetWeapon();
        this->InitializeState(m_player);
    }

    State::~State()
    {
        //dtor
    }


    void State::InitializeState(const Player& player)
    {
        const Room& room = player.GetRoom();

        for (size_t i = 0; i < room.GetTileCount(); ++i)
        {
            const ITile* tile = room.GetTile(i);
            switch (tile->GetTileType())
            {
            case TileType::Treasure:
                {
                    auto pTreasure = static_cast<const TileTreasure*>(tile);
                    if(!pTreasure->isEmpty())
                    {
                        StateNode* node = new StateNode(pTreasure);
                        CalculatePath(node, tile);
                        listNodes.push_back(*node);
                    }
                    break;
                }
            case TileType::Monster:
                {
                   auto pMonster = static_cast<const TileMonster*>(tile);
                   if(!pMonster->isEmpty())
                   {
                        StateNode* node = new StateNode(pMonster);
                        CalculatePath(node, tile);
                        listNodes.push_back(*node);
                   }
                    break;
                }
            case TileType::Floor:
            case TileType::Wall:

            default:
                break;

            }
        }
    }





    void State::CalculatePath(StateNode* node, const ITile* tile)
    {
        //Pour l'instant, le path finding se résume à aller dans la direction du trésor

        sf::Vector2i posPlayer = m_player.GetPosition();
        sf::Vector2i posObjet = tile->GetPosition();

        sf::Vector2i Diff = posObjet - posPlayer;

        node->SetDirection(DirectionFinding(Diff));

        float distance = sqrt(pow(float(Diff.x), 2) + pow(float(Diff.y), 2));
        node->SetDistance(int(distance));



    }

    Action::Direction State::DirectionFinding(const sf::Vector2i Diff)
    {

        float ratio = abs(float(Diff.y) / float(Diff.x));

        //Vérifier l'orientation du vecteur différentiel
        if(Diff.x > 0)
        {
            if(Diff.y > 0)
            {
                //Cadran bas-Droite
                if(ratio > 2)
                {
                    //cadran bas bas droite
                    return Action::Direction::Bottom;
                }
                else if (ratio < 0.5)
                {
                    //cadran bas droite droite
                    return Action::Direction::Right;
                }
                else
                {
                    return Action::Direction::BottomRight;
                }
            }
            else
            {
                //Cadran haut-Droite
                if(ratio > 2)
                {
                    //cadran haut haut droite
                    return Action::Direction::Top;
                }
                else if (ratio < 0.5)
                {
                    return Action::Direction::Right;
                }
                else
                {
                    return Action::Direction::TopRight;
                }
            }
        }
        else
        {
            if(Diff.y > 0)
            {
                //Cadran bas-Gauche
                if(ratio > 2)
                {
                    return Action::Direction::Bottom;
                }
                else if (ratio < 0.5)
                {
                    return Action::Direction::Left;
                }
                else
                {
                    return Action::Direction::BottomLeft;
                }
            }
            else
            {
                //Cadran haut-Gauche
                if(ratio > 2)
                {
                    return Action::Direction::Top;
                }
                else if (ratio < 0.5)
                {
                    return Action::Direction::Left;
                }
                else
                {
                    return Action::Direction::TopLeft;
                }
            }
        }
    }

    bool State::operator==(const State lhs) const
    {
        if(this->treasure != lhs.treasure)
            return false;
        if(this->strength != lhs.strength)
            return false;

        //this->listNodes.sort();
        //lhs.listNodes.sort();

        return (this->listNodes == lhs.listNodes);
    }
}

