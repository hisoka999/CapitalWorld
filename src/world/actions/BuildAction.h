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
            BuildAction(const std::shared_ptr<world::Company> &company, const std::shared_ptr<City> &city, const world::BuildingType buildingType, const std::shared_ptr<Product> &product);
            ~BuildAction();

            void execute(const std::shared_ptr<world::GameState> &gameState);

        private:
            /* data */
            std::shared_ptr<City> m_city;
            world::BuildingType m_buildingType;
            std::shared_ptr<Product> m_product;
        };

    } // namespace actions

} // namespace world
