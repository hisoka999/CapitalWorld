#pragma once

#include "Action.h"
#include "world/gamestate.h"

namespace world
{
    namespace actions
    {
        class BuildAction : public Action
        {
        public:
            BuildAction();
            ~BuildAction();

            void execute(const std::shared_ptr<world::GameState> &gameState, const std::shared_ptr<world::Company> &company);

        private:
            /* data */
        };

    } // namespace actions

} // namespace world
