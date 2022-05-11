#pragma once

#include "world/gamestate.h"
namespace world
{
    namespace actions
    {
        class Action
        {
        public:
            Action(const std::shared_ptr<world::Company> &company) : m_company(company){};
            virtual ~Action(){};

            virtual void execute(const std::shared_ptr<world::GameState> &gameState) = 0;
            std::shared_ptr<Action> nextAction();
            void setNextAction(const std::shared_ptr<Action> &action);

        protected:
            std::shared_ptr<world::Company> m_company;
            std::shared_ptr<world::City> findTargetCity(const std::shared_ptr<world::GameState> &gameState, const std::shared_ptr<world::Building> &building);

        private:
            std::shared_ptr<Action> m_action = nullptr;
        };

    } // namespace actions

} // namespace world
