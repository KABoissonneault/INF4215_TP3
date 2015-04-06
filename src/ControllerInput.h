#pragma once

#include "Controller.h"

namespace INF4215_TP3
{
    class ControllerInput : public IController
    {
    public:
        ControllerInput(const Player&);

        virtual std::unique_ptr<Action> ChooseAction() override;
    };
}
