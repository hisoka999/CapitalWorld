#include "Action.h"

namespace world
{
    namespace actions
    {
        std::shared_ptr<Action> Action::nextAction()
        {
            return m_action;
        }

        void Action::setNextAction(const std::shared_ptr<Action> &action)
        {
            if (m_action != nullptr)
            {
                m_action->setNextAction(action);
            }
            else
            {
                m_action = action;
            }
        }

        std::shared_ptr<world::City> Action::findTargetCity(const std::shared_ptr<world::GameState> &gameState, const std::shared_ptr<world::Building> &building)
        {
            std::shared_ptr<City> targetCity = nullptr;
            float distance = MAXFLOAT;
            for (auto &city : gameState->getCities())
            {
                float currentDistance = city->getPosition().distance(building->get2DPosition().toVecto2());
                if (currentDistance < distance)
                {
                    distance = currentDistance;
                    targetCity = city;
                }
            }
            return targetCity;
        }
    }
}