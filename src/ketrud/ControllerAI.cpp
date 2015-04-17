#include "ketrud/ControllerAI.h"

#include "ketrud/StateGame.h"

#include "Game.h"
#include "Player.h"
#include "Action.h"
#include "Room.h"
#include "TileTreasure.h"
#include "TileMonster.h"

#include <limits>

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
            m_pathFinder{GetRoom(), GetPlayer()},
            m_fCurrentReward{}
        {
            m_funPolicy = [this] () { return EpsilonGreedy(kfEpsilon); };
        }

        ControllerAI::~ControllerAI() = default;

        std::unique_ptr<Action> ControllerAI::ChooseAction()
        {
            if(m_pCurrentGoal)
            {
                if(!IsGoalValid())
                {
                    SearchNewGoal();
                    return ChooseAction();
                }

                Action::Direction eDir;
                if(m_pathFinder.PopDirection(eDir))
                {
                    return std::unique_ptr<Action>{new Action{GetPlayer(), eDir}};
                }
                else
                {
                    return nullptr;
                }
            }
            else
            {
                SearchNewGoal();
                return ChooseAction();
            }
        }

        void ControllerAI::SearchNewGoal()
        {
            m_pCurrentState = GenerateCurrentState();
            SetGoal(GetPolicyAction());
        }

        void ControllerAI::OnStun(unsigned nTurnCount)
        {
            (void) nTurnCount;
            SearchNewGoal();
        }

        void ControllerAI::OnTreasureChange(int nTreasureChange)
        {
            m_fCurrentReward += nTreasureChange;
        }


        void ControllerAI::OnTurnEnd()
        {
            if(IsGoalReached())
            {
                auto pNewState = GenerateCurrentState();
                double fNewQ = GetQValue(GetCurrentState(), *m_pCurrentGoal)
                    + (m_fCurrentReward + kfGamma * GetMaxQValue(*pNewState)
                    - GetQValue(GetCurrentState(), *m_pCurrentGoal));
                SetQValue(GetCurrentState(), *m_pCurrentGoal, fNewQ );

                m_pCurrentState = pNewState;

                m_pCurrentGoal = nullptr;
            }
        }

        void ControllerAI::SetGoal(const Goal& goal)
        {
            m_fCurrentReward = 0.0;
            m_pCurrentGoal.reset(new Goal(goal));

            m_pathFinder.FindPath(GetPlayer().GetPosition(), m_pCurrentGoal->Destination);
            m_fCurrentReward -= static_cast<double>(m_pathFinder.PathLength());
        }

        std::shared_ptr<const StateGame> ControllerAI::GenerateCurrentState()
        {
            const auto pCurrentState = std::make_shared<StateGame>(GetRoom(), GetPlayer()/*, GetOtherPlayer()*/);
            const auto itFind = m_setStates.find(pCurrentState);
            if(itFind == end(m_setStates))
            {
                m_pCurrentState = pCurrentState;
                return *(m_setStates.insert(pCurrentState).first);
            }
            else
            {
                m_pCurrentState = *itFind;
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

        double ControllerAI::GetQValue(std::shared_ptr<const StateGame> state, const Goal& goal)
        {
            StateAction stateAction{state, goal};
            const auto itFind = m_mapQValue.find(stateAction);
            if(itFind == end(m_mapQValue))
            {
                const auto itInsert = m_mapQValue.insert(std::make_pair(stateAction, 0.0));
                return itInsert.second;
            }
            else
            {
                return itFind->second;
            }
        }

        Goal ControllerAI::GetMaxQGoal(const StateGame& desiredState)
        {
            double fMaxQ = std::numeric_limits<double>::min();
            Goal currentMaxGoal;

            for(auto pairStateActionValue : m_mapQValue)
            {
                const auto stateAction = pairStateActionValue.first;

                const auto pCurrentState = stateAction.State;
                if(*pCurrentState == desiredState)
                {
                    const auto fCurrentQ = pairStateActionValue.second;
                    if(fCurrentQ > fMaxQ)
                    {
                        fMaxQ = fCurrentQ;
                        currentMaxGoal = stateAction.ChosenGoal;
                    }
                }
            }

            if(fMaxQ == std::numeric_limits<double>::min())
            {
                return GetRandomGoal();
            }
            else
            {
                return currentMaxGoal;
            }
        }

        double ControllerAI::GetMaxQValue(const StateGame& desiredState)
        {
            double fMaxQ = std::numeric_limits<double>::min();

            for(auto pairStateActionValue : m_mapQValue)
            {
                const auto stateAction = pairStateActionValue.first;

                const auto pCurrentState = stateAction.State;
                if(*pCurrentState == desiredState)
                {
                    const auto fCurrentQ = pairStateActionValue.second;
                    if(fCurrentQ > fMaxQ)
                    {
                        fMaxQ = fCurrentQ;
                    }
                }
            }

            if(fMaxQ == std::numeric_limits<double>::min())
            {
                return 0.0;
            }
            else
            {
                return fMaxQ;
            }
        }

        bool ControllerAI::IsGoalValid() const
        {
            auto pTile = GetRoom().GetTile(m_pCurrentGoal->Destination);
            if(pTile->GetTileType() == TileType::Treasure)
            {
                auto pTreasure = static_cast<const TileTreasure*>(pTile);
                return !pTreasure->isEmpty();
            }
            else
            {
                auto pMonster = static_cast<const TileMonster*>(pTile);
                return !pMonster->isEmpty();
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

        Goal ControllerAI::EpsilonGreedy(double epsilon)
        {
            static std::default_random_engine engine;
            std::uniform_real_distribution<double> dist(0.0, 1.0);

            const double v = dist(engine);
            if(v < epsilon)
            {
                return GetRandomGoal();
            }
            else
            {
                return GetMaxQGoal(*GetCurrentState());
            }
        }

        Goal ControllerAI::GetRandomGoal()
        {
            const ITile* pTile;
            while(true)
            {
                pTile = GetRoom().GetRandomTreasureOrMonster();
                if(pTile->GetTileType() == TileType::Treasure)
                {
                    auto pTreasure = static_cast<const TileTreasure*>(pTile);
                    if(!pTreasure->isEmpty())
                        break;
                }
                else
                {
                    auto pMonster = static_cast<const TileMonster*>(pTile);
                    if(!pMonster->isEmpty())
                        break;
                }
            }

            return {{pTile->GetPosition()}};
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
