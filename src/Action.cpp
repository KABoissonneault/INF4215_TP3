#include "Action.h"

#include "Player.h"

namespace INF4215_TP3
{
    sf::Vector2i Action::GetResultingPosition() const
    {
        sf::Vector2i posRet = SourcePlayer.GetPosition();
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

    bool Action::operator==(const Action& other) const
    {
        return ChosenDirection == other.ChosenDirection && SourcePlayer.GetID() == other.SourcePlayer.GetID();
    }
}

namespace std
{
    size_t hash<INF4215_TP3::Action>::operator()(const INF4215_TP3::Action& action) const
    {
        return (hash<unsigned>()(static_cast<unsigned>(action.ChosenDirection)) ^ (hash<unsigned>()(static_cast<unsigned>(action.SourcePlayer.GetID()) << 1))) >> 1;
    }
}
