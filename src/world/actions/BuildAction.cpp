#include "BuildAction.h"

namespace world
{
    namespace actions
    {
        BuildAction::BuildAction()
        {
        }

        BuildAction::~BuildAction()
        {
        }

        void BuildAction::execute(const std::shared_ptr<world::GameState> &gameState, const std::shared_ptr<world::Company> &company)
        {

            // find big city
            if (company->getBuildings().size() == 0)
            {
                // find area to build
                std::shared_ptr<City> currentCity = nullptr;

                for (auto &city : gameState->getCities())
                {
                    if (currentCity == nullptr || currentCity->getNumberOfCitizen() < city->getNumberOfCitizen())
                    {
                        currentCity = city;
                    }
                }
            }
        }
    }
}