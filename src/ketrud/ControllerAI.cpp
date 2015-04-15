#include "ketrud/ControllerAI.h"

#include "ketrud/StateGame.h"

#include "Player.h"
#include "Action.h"
#include "Room.h"

namespace INF4215_TP3
{
    namespace Ketrud
    {
        bool StateAction::operator==(const StateAction& other) const
        {
            return *State == *other.State && ChosenAction == other.ChosenAction;
        }

        ControllerAI::ControllerAI(const Player& player)
            : IController(player),
            // Initialisons l'état courrant à l'état initial
            m_pCurrentState(std::make_shared<StateGame>(player.GetRoom(), player, player.GetRoom().GetOtherPlayer(player)))
        {

        }

        ControllerAI::~ControllerAI() = default;

        std::unique_ptr<Action> ControllerAI::ChooseAction()
        {
            return nullptr;
        }

        void ControllerAI::OnStun(unsigned nTurnCount)
        {
            (void) nTurnCount;
        }

        void ControllerAI::OnTreasureChange(int nTreasureChange)
        {
            (void) nTreasureChange;
        }

        void ControllerAI::OnTurnEnd()
        {

        }
    }
}

namespace std
{
    size_t hash<INF4215_TP3::Ketrud::StateAction>::operator()(const INF4215_TP3::Ketrud::StateAction& stateAction) const
    {
        return (hash<INF4215_TP3::Ketrud::StateGame>()(*stateAction.State) ^ (hash<INF4215_TP3::Action>()(stateAction.ChosenAction) << 1) ) >> 1;
    }
}
