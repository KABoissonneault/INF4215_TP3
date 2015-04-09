#pragma once

#include <SFML/System/Vector2.hpp>


namespace INF4215_TP3
{
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

        sf::Vector2i GetResultingPosition(const sf::Vector2i& pos)
        {
            sf::Vector2i posRet = pos;
            const unsigned dir = static_cast<unsigned>(ChosenDirection);
            if(dir & static_cast<unsigned>(Direction::Top))
            {
                --posRet.y;
            }
            else if(dir & static_cast<unsigned>(Direction::Bottom))
            {
                ++posRet.y;
            }

            if(dir & static_cast<unsigned>(Direction::Left))
            {
                --posRet.x;
            }
            else if(dir & static_cast<unsigned>(Direction::Right))
            {
                ++posRet.x;
            }

            return posRet;
        }

        Direction ChosenDirection;
    };
}
