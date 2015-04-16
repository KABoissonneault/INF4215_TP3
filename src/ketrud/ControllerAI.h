#pragma once

#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "Controller.h"
#include "Action.h"
#include "Player.h"
#include "ketrud/AStar.h"

namespace INF4215_TP3
{
    namespace Ketrud
    {
        struct StateAction;
    }
}

namespace std
{
    template<>
    struct hash<INF4215_TP3::Ketrud::StateAction>
    {
        size_t operator()(const INF4215_TP3::Ketrud::StateAction&) const noexcept;
    };
}

namespace INF4215_TP3
{
    namespace Ketrud
    {
        class StateGame;

        struct StatePtrHash
        {
            size_t operator()(const std::shared_ptr<StateGame>& pState) const;
        };

        struct StatePtrEqual
        {
            bool operator()(const std::shared_ptr<StateGame>& pState1, const std::shared_ptr<StateGame>& pState2) const;
        };

        struct Goal
        {
            sf::Vector2i Destination;
        };

        struct StateAction
        {
            std::shared_ptr<const StateGame> State;
            Goal ChosenGoal;

            bool operator==(const StateAction&) const;
        };

        class ControllerAI final : public IController
        {
        public:
            ControllerAI(const Player&);
            ~ControllerAI();

            virtual std::unique_ptr<Action> ChooseAction() override;

            virtual void OnStun(unsigned nTurnCount) override;
            virtual void OnTreasureChange(int nTreasureChange) override;
            virtual void OnTurnEnd() override;

        private:
            static constexpr double kfEpsilon = 0.3;
            Goal EpsilonGreedy(double fEpsilon);

            Goal GetPolicyAction()
            {
                return m_funPolicy();
            }

            std::shared_ptr<const StateGame> GetCurrentState();
            void SetQValue(std::shared_ptr<const StateGame>, const Goal&, double value);

            const Room& GetRoom() const
            {
                return GetPlayer().GetRoom();
            }

            const Player& GetOtherPlayer() const
            {
                return m_OtherPlayer;
            }

            const Player& m_OtherPlayer;

            // Q learning structures
            std::unordered_map<StateAction, double> m_mapQValue;

            // Cache d'états
            std::unordered_set<std::shared_ptr<StateGame>, StatePtrHash, StatePtrEqual> m_setStates;
            AStar m_pathFinder;

            // Données de l'état courrant du AI
            std::shared_ptr<const StateGame> m_pCurrentState;
            //std::unique_ptr<Action> m_pCurrentAction;
            std::unique_ptr<Goal> m_pCurrentGoal;
            double m_fCurrentReward;
            std::function<Goal()> m_funPolicy;

            static constexpr double kfGamma = 0.9;
        };


    }
}


