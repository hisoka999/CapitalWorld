#pragma once

#include "Action.h"

namespace world
{
    namespace actions
    {

        class ResearchWorkerAction : public Action
        {
        public:
            ResearchWorkerAction(const std::shared_ptr<world::Company> &company);
            virtual void execute(const std::shared_ptr<world::GameState> &gameState) override;
            virtual bool canExecute(const std::shared_ptr<world::GameState> &gameState) override;
        };

    } // namespace actions

} // namespace world
