#pragma once

#include <SFML/System/Vector2.hpp>

#include <cmath>

namespace sf
{
    template<class T>
    double Norm(const Vector2<T>& vec)
    {
        return std::sqrt(SquaredNorm(vec));
    }

    template<class T>
    double SquaredNorm(const Vector2<T>& vec)
    {
        return static_cast<double>(vec.x*vec.x + vec.y*vec.y);
    }

    template<class T>
    sf::Vector2<T> Normalized(const Vector2<T>& vec)
    {
        return vec/Norm(vec);
    }

    template<class T>
    bool operator<(const sf::Vector2<T>& first, const sf::Vector2<T>& second)
    {
        return first.x < second.x ?
            true
            : first.x == second.x ?
                first.y < second.y
                : false;
    }
}
