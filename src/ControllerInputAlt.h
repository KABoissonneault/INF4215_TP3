#pragma once

#include "Controller.h"

namespace INF4215_TP3
{
    class ControllerInputAlt : public IController
    {
    public:
        ControllerInputAlt(const Player&);

        virtual std::unique_ptr<Action> ChooseAction() override;
    };
}
