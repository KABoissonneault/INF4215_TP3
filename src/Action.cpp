#include "Action.h"

#include "Player.h"

#include <mutex>

namespace INF4215_TP3
{
    namespace
    {
        std::once_flag flagDirections;
    }

    std::vector<Action::Direction> Action::s_vecDirections;
    auto Action::GetDirections() -> const std::vector<Direction>&
    {
        std::call_once(flagDirections, [] ()
        {
            s_vecDirections.push_back(Direction::Idle);
            s_vecDirections.push_back(Direction::Top);
            s_vecDirections.push_back(Direction::Bottom);
            s_vecDirections.push_back(Direction::Left);
            s_vecDirections.push_back(Direction::Right);
            s_vecDirections.push_back(Direction::TopLeft);
            s_vecDirections.push_back(Direction::TopRight);
            s_vecDirections.push_back(Direction::BottomLeft);
            s_vecDirections.push_back(Direction::BottomRight);
        });

        return s_vecDirections;
    }

    sf::Vector2i operator+(const sf::Vector2i& pos, const Action::Direction& eDir)
    {
        sf::Vector2i posRet = pos;
        const unsigned dir = static_cast<unsigned>(eDir);
        if(dir & static_cast<unsigned>(Action::Direction::Top))
        {
            --posRet.y;
        }
        else if(dir & static_cast<unsigned>(Action::Direction::Bottom))
        {
            ++posRet.y;
        }

        if(dir & static_cast<unsigned>(Action::Direction::Left))
        {
            --posRet.x;
        }
        else if(dir & static_cast<unsigned>(Action::Direction::Right))
        {
            ++posRet.x;
        }

        return posRet;
    }

    sf::Vector2i Action::GetResultingPosition() const
    {
        return SourcePlayer.GetPosition() + ChosenDirection;
    }

    bool Action::operator==(const Action& other) const
    {
        return ChosenDirection == other.ChosenDirection && SourcePlayer.GetID() == other.SourcePlayer.GetID();
    }
}

namespace std
{
    size_t hash<INF4215_TP3::Action>::operator()(const INF4215_TP3::Action& action) const noexcept
    {
        return (hash<unsigned>()(static_cast<unsigned>(action.ChosenDirection)) ^ (hash<unsigned>()(static_cast<unsigned>(action.SourcePlayer.GetID()) << 1))) >> 1;
    }
}
