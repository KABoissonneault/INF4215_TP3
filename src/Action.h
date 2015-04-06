#pragma once

namespace INF4215_TP3
{
    struct Action
    {
        enum class Direction
        {
            Idle = 0,
            Top = 1,
            Bottom = 2,
            Left = 4,
            Right = 8,
            TopLeft = Top|Left,
            TopRight = Top|Right,
            BottomLeft = Bottom|Left,
            BottomRight = Bottom|Right
        };

        Direction ChosenDirection;
    };
}
