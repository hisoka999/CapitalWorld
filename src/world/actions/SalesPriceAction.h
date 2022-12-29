#pragma once

#include "Action.h"

namespace world
{
    namespace actions
    {
        class SalesPriceAction : public Action
        {

        public:
            SalesPriceAction(const std::shared_ptr<world::Company> &company);
            ~SalesPriceAction();
            virtual void execute(const std::shared_ptr<world::GameState> &gameState);

            virtual bool canExecute(const std::shared_ptr<world::GameState> &gameState);
        };

    } // namespace actions
}