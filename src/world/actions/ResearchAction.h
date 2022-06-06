#pragma once

#include "Action.h"

namespace world
{
    namespace actions
    {

        class ResearchAction : public Action
        {
        public:
            ResearchAction(const std::shared_ptr<world::Company> &company, const std::shared_ptr<world::Building> &building);
            virtual void execute(const std::shared_ptr<world::GameState> &gameState);

        private:
            std::shared_ptr<world::Building> m_building;
        };

    } // namespace actions

} // namespace world
