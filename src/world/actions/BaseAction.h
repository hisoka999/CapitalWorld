#pragma once

#include "Action.h"

namespace world
{
    namespace actions
    {

        class BaseAction : public Action
        {
        public:
            BaseAction(const std::shared_ptr<world::Company> &company);
            virtual void execute(const std::shared_ptr<world::GameState> &gameState);
        };

    } // namespace actions

} // namespace world
