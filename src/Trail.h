#pragma once

#include <SFML/Graphics.hpp>

namespace INF4215_TP3
{
    class Player;
    class Action;

    class Trail
    {
    public:
        // Player must be valid for the length of the constructor. Won't be cached
        Trail(const Player&, const Action&);

        void Render(sf::RenderTarget& view, const sf::Transform& context);

    private:
        static sf::Texture s_textPlayer1Trail;
        static sf::Texture s_textPlayer2Trail;

        sf::Sprite m_spriteSourceTrail;
        sf::Sprite m_spriteSourceOutTrail;
        sf::Sprite m_spriteDestinationInTrail;
    };
}
