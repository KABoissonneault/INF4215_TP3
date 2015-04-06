#include "ControllerInput.h"

#include "Game.h"
#include "Player.h"
#include "Action.h"

namespace INF4215_TP3
{
    ControllerInput::ControllerInput(const Player& player)
        : IController(player)
    {

    }

    std::unique_ptr<Action> ControllerInput::ChooseAction()
    {
        for(unsigned i = 1; i <= 9; ++i)
        {
            if(i != 5 && Game::Instance().NumpadWasPressed(i))
            {
                unsigned nDir = static_cast<unsigned>(Action::Direction::Idle);
                switch((i-1)/3)
                {
                case 0:
                    nDir |= static_cast<unsigned>(Action::Direction::Bottom);
                    break;
                case 2:
                    nDir |= static_cast<unsigned>(Action::Direction::Top);
                    break;
                default:
                    break;
                }
                switch((i-1)%3)
                {
                case 0:
                    nDir |= static_cast<unsigned>(Action::Direction::Left);
                    break;
                case 2:
                    nDir |= static_cast<unsigned>(Action::Direction::Right);
                    break;
                default:
                    break;
                }
                Action::Direction eDir = static_cast<Action::Direction>(nDir);
                return std::unique_ptr<Action>{ new Action{ eDir } };
            }
        }

        return nullptr;
    }
}
