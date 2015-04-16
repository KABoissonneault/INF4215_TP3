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
            return *State == *other.State && ChosenGoal.Destination == other.ChosenGoal.Destination;
        }

        ControllerAI::ControllerAI(const Player& player)
            : IController{player},
            m_OtherPlayer{player.GetRoom().GetOtherPlayer(player)},
            m_pathFinder{GetRoom()},
            m_fCurrentReward{}
        {
            m_funPolicy = [this] () { return EpsilonGreedy(kfEpsilon); };
        }

        ControllerAI::~ControllerAI() = default;

        std::unique_ptr<Action> ControllerAI::ChooseAction()
        {
            if(m_pCurrentGoal)
            {
                Action::Direction eDir;
                if(m_pathFinder.PopDirection(eDir))
                {
                    return std::unique_ptr<Action>{new Action{GetPlayer(), eDir}};
                }
                else
                {
                    m_pCurrentGoal.reset(new Goal{GetPolicyAction()});
                    return ChooseAction();
                }
            }
            else
            {
                m_pCurrentGoal.reset(new Goal{GetPolicyAction()});
                return ChooseAction();
            }
        }

        void ControllerAI::OnStun(unsigned nTurnCount)
        {
            m_fCurrentReward -= nTurnCount / 2.0;
        }

        void ControllerAI::OnTreasureChange(int nTreasureChange)
        {
            m_fCurrentReward += nTreasureChange;
        }

        void ControllerAI::OnTurnEnd()
        {
            //auto pNewState = GetCurrentState();
            //SetQValue(m_pCurrentState, *m_pCurrentGoal, 1.0);
        }

        std::shared_ptr<const StateGame> ControllerAI::GetCurrentState()
        {
            const auto pCurrentState = std::make_shared<StateGame>(GetRoom(), GetPlayer(), GetOtherPlayer());
            const auto itFind = m_setStates.find(pCurrentState);
            if(itFind == end(m_setStates))
            {
                return *(m_setStates.insert(pCurrentState).first);
            }
            else
            {
                return *itFind;
            }
        }

        void ControllerAI::SetQValue(std::shared_ptr<const StateGame> state, const Goal& goal, double value)
        {
            StateAction stateAction{state, goal};
            const auto itFind = m_mapQValue.find(stateAction);
            if(itFind == end(m_mapQValue))
            {
                m_mapQValue.insert(std::make_pair(stateAction, value));
            }
            else
            {
                itFind->second = value;
            }
        }

        size_t StatePtrHash::operator()(const std::shared_ptr<StateGame>& pState) const
        {
            return std::hash<StateGame>()(*pState);
        }

        bool StatePtrEqual::operator()(const std::shared_ptr<StateGame>& pState1, const std::shared_ptr<StateGame>& pState2) const
        {
            return *pState1 == *pState2;
        }

        Goal ControllerAI::EpsilonGreedy(double)
        {
            return {{0,0}};
        }
    }
}

namespace std
{
    size_t hash<INF4215_TP3::Ketrud::StateAction>::operator()(const INF4215_TP3::Ketrud::StateAction& stateAction) const noexcept
    {
        return (hash<INF4215_TP3::Ketrud::StateGame>()(*stateAction.State) ^ ((stateAction.ChosenGoal.Destination.x << 1) ^ (stateAction.ChosenGoal.Destination.y) << 2)) >> 1;
    }
}
