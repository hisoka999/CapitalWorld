#pragma once
#include "Action.h"
namespace world
{
    namespace actions
    {
        class LoanAction : public Action
        {
        public:
            LoanAction(const std::shared_ptr<world::Company> &company);
            ~LoanAction();
            virtual void execute(const std::shared_ptr<world::GameState> &gameState);

            virtual bool canExecute(const std::shared_ptr<world::GameState> &gameState);

        private:
            /* data */
        };

    } // namespace actions

} // namespace world
