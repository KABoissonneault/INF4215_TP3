#pragma once

#include <SFML/System/Vector2.hpp>

#include <functional>

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

        bool operator==(const Action&) const;

        const Player& SourcePlayer;
        const Direction ChosenDirection;
    };
}

namespace std
{
    template<>
    struct hash<INF4215_TP3::Action>
    {
        size_t operator()(const INF4215_TP3::Action& action) const;
    };
}
