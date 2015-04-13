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
}
