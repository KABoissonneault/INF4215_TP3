#pragma once

#include <SFML/System/Vector2.hpp>


namespace INF4215_TP3
{
    class Player;

    struct Action
    {
        enum class Direction : unsigned
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

        sf::Vector2i GetResultingPosition() const;

        const Player& SourcePlayer;
        const Direction ChosenDirection;
    };
}
