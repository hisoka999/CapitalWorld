#pragma once

#include "Action.h"

namespace world
{
    namespace actions
    {

        class ProductionAction : public Action
        {
        public:
            ProductionAction(const std::shared_ptr<world::Company> &company, const std::shared_ptr<world::Building> &building, const std::shared_ptr<Product> &product);
            virtual void execute(const std::shared_ptr<world::GameState> &gameState);
            virtual bool canExecute(const std::shared_ptr<world::GameState> &gameState);

        private:
            std::shared_ptr<world::Building> m_building;
            std::shared_ptr<Product> m_product;
        };

    } // namespace actions

} // namespace world
