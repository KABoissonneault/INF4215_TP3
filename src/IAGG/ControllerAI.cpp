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
#include "AStar.h"

#include <vector>
#include <list>


namespace INF4215_TP3
{
    const float GAMMA = 0.9f;
    //const float epsilon = 0.1f;

    ControllerAI::ControllerAI(const Player& player, const Player& other)
    : IController(player), OtherPlayer{other}, currentState{}, pathfinder(player.GetRoom(), player)
    {


    }

    std::unique_ptr<Action> ControllerAI::ChooseAction()
    {
        State* state = new State(m_Player);
        unsigned Target = state->Count() - 1;
        //std::uniform_real_distribution<float> distribution(0,1);


        //Check for reward
        // Way to know if won.

        int r = rand();

        //CHIFFRE ENTRE 0 ET 1
        float v = (r % 1000) / 1000.0f;

        int maxQ = 0;
        for ( unsigned tempTarget = 0; tempTarget < state->Count(); tempTarget++)
        {

                 float q = this->GetNode(*state,tempTarget).getQ();
                 if(q > maxQ)
                 {
                     maxQ = q;
                     Target = tempTarget;
                 }

        }


        if( Target != currentTarget && v < GetNode(*state, Target).getA())
        {
            Target = rand() % state->Count();
        }

        //Set Q of last StateAction
        if(this->currentState)
        {
            QNode currentNode = GetNode(*currentState,currentTarget);
            currentNode.setQ
            (
                currentNode.getQ() +
                currentNode.getA() * (GAMMA * (maxQ - currentNode.getQ()))
            );
        }

        Action::Direction direc;
        //Aller vers le target
        if (Target != this->currentTarget || this->currentState)
        {
            if(!pathfinder.FindPath(m_Player.GetPosition(), state->GetPosition(Target)))
            {
                direc = Action::Direction::Idle;
            }
        }
        if(!pathfinder.PopDirection(direc))
        {
            pathfinder.FindPath(m_Player.GetPosition(), state->GetPosition(Target));
            if(!pathfinder.PopDirection(direc))
            {
                direc = Action::Direction::Idle;
            }
        }

        std::unique_ptr<Action> action = std::unique_ptr<Action>{ new Action{ m_Player, direc } };

        this->currentState = state;
        this->currentTarget = Target;
        return action;
    }

     void ControllerAI::OnGameEnd(bool hasWon)
     {
         StateAction sa = StateAction(*currentState,currentTarget);
         this->LearningNodes[sa].setQ( hasWon? 100: -100);
     }
/*
    float ControllerAI::GetQ(State state, Action action)
    {
        return LearningNodes[StateAction(state, action)].getQ();
    }

    float ControllerAI::GetA(State state, Action action)
    {
        return LearningNodes[StateAction(state, action)].getA();
    }

    float ControllerAI::GetN(State state, Action action)
    {
        return LearningNodes[StateAction(state, action)].getN();
    }

    void ControllerAI::SetQ(State state, Action action, float q)
    {
        if(LearningNodes.c)
        LearningNodes[StateAction(state,action)].setQ(q);
    }
*/

    QNode ControllerAI::GetNode(State state, unsigned target)
    {
        StateAction sa = StateAction(state,target);
        std::map<StateAction,QNode>::iterator it = this->LearningNodes.find(sa);
        if(it == this->LearningNodes.end())
        {
            this->LearningNodes[sa] = QNode();
        }
        return this->LearningNodes[sa];


    }
/*
    void ControllerAI::SetA(State state, Action action, float a)
    {
        LearningNodes[StateAction(state,action)].A = a;
    }

    void ControllerAI::SetN(State state, Action action, float n)
    {
        LearningNodes[StateAction(state,action)].N = n;
    }
*/
}
