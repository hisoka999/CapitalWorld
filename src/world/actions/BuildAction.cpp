#include "BuildAction.h"
#include <services/buildingservice.h>
#include <random>
#include <algorithm>
#include <world/actions/ProductionAction.h>

namespace world
{
    namespace actions
    {
        static std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());

        BuildAction::BuildAction(const std::shared_ptr<world::Company> &company, const std::shared_ptr<City> &city, const world::BuildingType buildingType, const std::shared_ptr<Product> &product)
            : Action(company), m_city(city), m_buildingType(buildingType), m_product(product)
        {
        }

        BuildAction::~BuildAction()
        {
        }

        void BuildAction::execute(const std::shared_ptr<world::GameState> &gameState)
        {
            std::shared_ptr<world::Building> targetBuilding = nullptr;
            std::shared_ptr<City> currentCity = m_city;

            // select building to build
            std::map<world::BuildingType, size_t> usage;
            for (auto &building : m_company->getBuildings())
            {
                usage[building->getType()]++;
            }

            size_t minUsage = 0;
            for (auto u : usage)
            {
                minUsage = std::min(u.second, minUsage);
            }
            if (m_product != nullptr)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(m_buildingType)[0];
            }
            else if (usage.count(world::BuildingType::Shop) == minUsage)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(world::BuildingType::Shop)[0];
            }
            else if (usage.count(world::BuildingType::Factory) == minUsage)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(world::BuildingType::Factory)[0];
            }
            else if (usage.count(world::BuildingType::Resource) == minUsage)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(world::BuildingType::Resource)[0];
            }
            else if (usage.count(world::BuildingType::Transport) == minUsage)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(world::BuildingType::Transport)[0];
            }
            if (!targetBuilding)
                return;

            targetBuilding = services::BuildingService::Instance().create(targetBuilding);
            for (auto &street : currentCity->getStreets())
            {
                auto pos = street->get2DPosition();
                auto northBuilding = gameState->getGameMap()->getBuilding2D({pos.x, pos.y - 1, pos.width, 1});
                auto southBuilding = gameState->getGameMap()->getBuilding2D({pos.x, pos.y + pos.height, 1, pos.height});
                auto eastBuilding = gameState->getGameMap()->getBuilding2D({pos.x + pos.width, pos.y, pos.width, 1});
                auto westBuilding = gameState->getGameMap()->getBuilding2D({pos.x - 1, pos.y, 1, pos.height});

                if (northBuilding == nullptr)
                {
                    targetBuilding->setPosition(pos.x, pos.y - 1);
                }
                else if (southBuilding == nullptr)
                {
                    targetBuilding->setPosition(pos.x, pos.y + pos.height);
                }
                else if (eastBuilding == nullptr)
                {
                    targetBuilding->setPosition(pos.x + pos.width, pos.y);
                }
                else if (westBuilding == nullptr)
                {
                    targetBuilding->setPosition(pos.x - 1, pos.y);
                }

                if (targetBuilding->canBuild(m_company->getCash()) && gameState->getGameMap()->canBuild(targetBuilding->get2DPosition()))
                {
                    m_company->addBuilding(targetBuilding);
                    gameState->getGameMap()->addBuilding(targetBuilding);
                    m_company->incCash(targetBuilding->getBuildPrice() * -1);
                    auto productionAction = std::make_shared<world::actions::ProductionAction>(m_company, targetBuilding, m_product);
                    setNextAction(productionAction);
                    break;
                }
            }
        }
    }
}