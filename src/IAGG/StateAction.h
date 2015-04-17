#pragma once

#include "State.h"
#include "Action.h"

namespace INF4215_TP3
{
    class State;
    class Action;

    struct StateAction
    {
        State m_state;
        Action m_action;

        StateAction(State state, Action action)
        :m_state(state), m_action(action)
        {

        }
    };

    struct StateActionComparator
    {
        bool operator()(StateAction lhs, StateAction rhs) const
        {
            return lhs.m_state == rhs.m_state && rhs.m_action == lhs.m_action;
        }
    };
}

