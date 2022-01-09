#pragma once

#include "world/gamestate.h"
namespace world
{
    namespace actions
    {
        class Action
        {
        public:
            Action(){};
            virtual ~Action(){};

            virtual void execute(const std::shared_ptr<world::GameState> &gameState, const std::shared_ptr<world::Company> &company) = 0;

        private:
            /* data */
        };

    } // namespace actions

} // namespace world
