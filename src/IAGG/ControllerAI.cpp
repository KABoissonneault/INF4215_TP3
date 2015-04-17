#include "ControllerAI.h"

#include "Game.h"
#include "Player.h"
#include "Action.h"
#include "Room.h"
#include "State.h"
#include "StateNode.h"
#include "Action.h"
#include "StateAction.h"
#include "Qnode.h"

#include <vector>
#include <list>


namespace INF4215_TP3
{
    ControllerAI::ControllerAI(const Player& player, const Player& other)
    : IController(player), OtherPlayer{other}
    {

    }

    std::unique_ptr<Action> ControllerAI::ChooseAction()
    {
        State* state = new State(m_Player);
        std::unique_ptr<Action> action = std::unique_ptr<Action>{ new Action{ m_Player, Action::Direction::Idle } };
        //std::uniform_real_distribution<float> distribution(0,1);

        int r = rand();

        //CHIFFRE ENTRE 0 ET 1
        float v = (r % 1000) / 1000.0f;

        if(v < epsilon)
        {
            //Choisir action aléatoire
            bool ActionValide = true;
            do
            {
                int act = rand();
                action->ChosenDirection = Action::Direction(act % 8);

                auto tile = m_Player.GetRoom().GetTile(action->GetResultingPosition());
                if(tile->isSolid())
                {
                    ActionValide = false;
                }
                else
                {
                    ActionValide = true;
                }

            }
            while(ActionValide);

        }
        else
        {
            //Choisir la meilleure action


            auto listActions = Action::GetDirections();

            for ( std::vector<Action::Direction>::iterator iter = listActions.begin(); iter != listActions.end(); ++iter)
            {
                //Checker tous les actions et chjoisir la meilleur possible

            }
        }

        return action;
    }

    unsigned ControllerAI::CompareStrength()
    {
        //Retourne un nombre positif si le joueur du joueur est plus grande que la force de l'autre joueur
        return m_Player.GetWeapon() - OtherPlayer.GetWeapon();
    }

    const TileTreasure* ControllerAI::NearestTreasure()
    {
        auto& room = m_Player.GetRoom();
        const TileTreasure* nearestTreasure;
        for(size_t i = 0; i < room.GetTileCount(); ++i)
        {
            if(room.GetTile(i)->isTreasure())
            {
                //TODO calculer distance
                nearestTreasure = dynamic_cast<const TileTreasure*>(room.GetTile(i));
            }
        }

        return nearestTreasure;
    }

    unsigned ControllerAI::PathFinding(ITile* tile)
    {
        sf::Vector2i pos = m_Player.GetPosition();
        return 3;
    }

    float ControllerAI::GetQ(State state, Action action)
    {
        return LearningNodes[StateAction(state, action)].Q;
    }

    float ControllerAI::GetA(State state, Action action)
    {
        return LearningNodes[StateAction(state, action)].A;
    }

    float ControllerAI::GetN(State state, Action action)
    {
        return LearningNodes[StateAction(state, action)].N;
    }

    void ControllerAI::SetQ(State state, Action action, float q)
    {
        LearningNodes[StateAction(state,action)].Q = q;
    }

    void ControllerAI::SetA(State state, Action action, float a)
    {
        LearningNodes[StateAction(state,action)].A = a;
    }

    void ControllerAI::SetN(State state, Action action, float n)
    {
        LearningNodes[StateAction(state,action)].N = n;
    }

}
