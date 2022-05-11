#include "BaseAction.h"
#include <random>
#include <algorithm>
#include "BuildAction.h"
#include "ProductionAction.h"
#include <iostream>

namespace world
{
    namespace actions
    {
        static std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());

        BaseAction::BaseAction(const std::shared_ptr<world::Company> &company) : Action(company)
        {
        }

        void BaseAction::execute(const std::shared_ptr<world::GameState> &gameState)
        {
            if (m_company->getBuildings().size() == 0)
            {

                std::shared_ptr<City> currentCity = nullptr;

                // find big city
                if (m_company->getBuildings().size() == 0)
                {
                    // find area to build
                    auto cities = gameState->getCities();

                    std::shuffle(cities.begin(), cities.end(), gen);
                    currentCity = cities[0];
                }
                else
                {
                    auto pos = m_company->getBuildings().at(0)->get2DPosition();
                    float minDistance = std::numeric_limits<float>::max();
                    for (auto &city : gameState->getCities())
                    {
                        float distance = pos.toVecto2().distance(city->getPosition());
                        if (minDistance > distance)
                        {
                            minDistance = distance;
                            currentCity = city;
                        }
                    }
                }
                std::shared_ptr<Action> buildShopAction = std::make_shared<world::actions::BuildAction>(m_company, currentCity, world::BuildingType::Shop, nullptr);
                setNextAction(buildShopAction);

                // build shop

                // set shop production
                // the selected product needs to be the base for further acitons
                // set production for X -> build factory for X -> start production of x at factory
                //
                // build factory

                // set production

                // build resource building(s)

                // set production

                // build transport office

                // set transport
            }
            else
            {
                std::shared_ptr<City> currentCity = nullptr;

                auto pos = m_company->getBuildings().at(0)->get2DPosition();
                float minDistance = std::numeric_limits<float>::max();
                for (auto &city : gameState->getCities())
                {
                    float distance = pos.toVecto2().distance(city->getPosition());
                    if (minDistance > distance)
                    {
                        minDistance = distance;
                        currentCity = city;
                    }
                }
                // check production update
                //  new city? , more production ???
                std::shared_ptr<Action> buildAction = std::make_shared<world::actions::BuildAction>(m_company, currentCity, world::BuildingType::Transport, nullptr);
                setNextAction(buildAction);
                bool noTransport = true;
                for (auto &building : m_company->getBuildings())
                {
                    switch (building->getType())
                    {
                    case BuildingType::Transport:
                    {
                        std::shared_ptr<Action> transportAction = std::make_shared<world::actions::ProductionAction>(m_company, building, nullptr);
                        setNextAction(transportAction);
                        noTransport = false;
                        break;
                    }
                    default:
                        break;
                    }
                }
                if(noTransport){
                    std::cout<<"company has no transport building: "<<m_company->getName()<<std::endl;
                    std::cout<<"city: "<<currentCity->getName()<<std::endl; 
                }
            }
        }
    }
}