#include "ControllerInputAlt.h"

#include "Game.h"
#include "Player.h"
#include "Action.h"

namespace INF4215_TP3
{
    ControllerInputAlt::ControllerInputAlt(const Player& player)
        : IController(player)
    {

    }

    std::unique_ptr<Action> ControllerInputAlt::ChooseAction()
    {
        if(Game::Instance().LetterWasPressed('q'))
        {
            return std::unique_ptr<Action>{new Action{m_Player, Action::Direction::TopLeft}};
        }
        else if(Game::Instance().LetterWasPressed('w'))
        {
            return std::unique_ptr<Action>{new Action{m_Player, Action::Direction::Top}};
        }
        else if(Game::Instance().LetterWasPressed('e'))
        {
            return std::unique_ptr<Action>{new Action{m_Player, Action::Direction::TopRight}};
        }
        else if(Game::Instance().LetterWasPressed('a'))
        {
            return std::unique_ptr<Action>{new Action{m_Player, Action::Direction::Left}};
        }
        else if(Game::Instance().LetterWasPressed('s'))
        {
            return std::unique_ptr<Action>{new Action{m_Player, Action::Direction::Idle}};
        }
        else if(Game::Instance().LetterWasPressed('d'))
        {
            return std::unique_ptr<Action>{new Action{m_Player, Action::Direction::Right}};
        }
        else if(Game::Instance().LetterWasPressed('z'))
        {
            return std::unique_ptr<Action>{new Action{m_Player, Action::Direction::BottomLeft}};
        }
        else if(Game::Instance().LetterWasPressed('x'))
        {
            return std::unique_ptr<Action>{new Action{m_Player, Action::Direction::Bottom}};
        }
        else if(Game::Instance().LetterWasPressed('c'))
        {
            return std::unique_ptr<Action>{new Action{m_Player, Action::Direction::BottomRight}};
        }

        return nullptr;
    }
}
