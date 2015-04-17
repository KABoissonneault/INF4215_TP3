#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>
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

        static const std::vector<Direction>& GetDirections();

        sf::Vector2i GetResultingPosition() const;

        const Player& SourcePlayer;
        Direction ChosenDirection;

        bool operator==(const Action& rhs) const;

    private:
        static std::vector<Direction> s_vecDirections;

    };
    sf::Vector2i operator+(const sf::Vector2i& pos, const Action::Direction& action);
}

namespace std
{
    template<>
    struct hash<INF4215_TP3::Action>
    {
        size_t operator()(const INF4215_TP3::Action& action) const noexcept;
    };
}
