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
        unsigned m_target;

        StateAction(State state, unsigned target)
        :m_state(state), m_target(target)
        {

        }
    };

    struct StateActionComparator
    {
        bool operator()(StateAction lhs, StateAction rhs) const
        {
            return lhs.m_state == rhs.m_state && rhs.m_target == lhs.m_target;
        }
    };
}

