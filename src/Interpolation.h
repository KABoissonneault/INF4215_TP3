#pragma once

#include <SFML/System/Vector2.hpp>

namespace INF4215_TP3
{
    namespace Utility
    {
        sf::Vector2f WeightedAverageInterpolate(sf::Vector2f currentPosition, sf::Vector2f targetPosition, float weight)
        {
            return ((currentPosition * (weight - 1)) + targetPosition) / weight;
        }
    }
}
