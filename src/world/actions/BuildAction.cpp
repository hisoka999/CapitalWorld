#include "BuildAction.h"
#include <services/buildingservice.h>
#include <random>
#include <algorithm>
#include <world/actions/ProductionAction.h>
#include <magic_enum.hpp>

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

            size_t minUsage = (usage.size() > 0) ? (*usage.begin()).second : 0;

            for (auto type : magic_enum::enum_values<world::BuildingType>())
            {
                if (type == world::BuildingType::Street || type == world::BuildingType::House || type == world::BuildingType::Other)
                    continue;

                if (usage.count(type) == 0)
                {
                    usage[type] = 0;
                    // continue;
                }
                minUsage = std::min(usage[type], minUsage);
            }
            if (m_buildingType == world::BuildingType::Street)
            {
                targetBuilding = std::make_shared<world::buildings::Street>();
                graphics::Rect rect;
                rect.x = 0;
                rect.y = 128;
                rect.width = 64;
                rect.height = 32;
                targetBuilding->setOffset(0, 0);
                targetBuilding->setSourceRect(rect);
            }
            else if (m_product != nullptr)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(m_buildingType)[0];
            }
            else if (usage[world::BuildingType::Shop] == minUsage)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(world::BuildingType::Shop)[0];
            }
            else if (usage[world::BuildingType::Factory] == minUsage)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(world::BuildingType::Factory)[0];
            }
            else if (usage[world::BuildingType::Resource] == minUsage)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(world::BuildingType::Resource)[0];
            }
            else if (usage[world::BuildingType::Transport] == minUsage)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(world::BuildingType::Transport)[0];
            }
            else if (usage[world::BuildingType::Other] == minUsage && usage[world::BuildingType::Other] <= 2)
            {
                targetBuilding = m_company->findAvailableBuildingsByType(world::BuildingType::Other)[0];
            }
            if (!targetBuilding)
                return;
            if (targetBuilding->getType() != world::BuildingType::Street)
                targetBuilding = services::BuildingService::Instance().create(targetBuilding);
            bool foundPosition = false;
            auto streets = currentCity->getStreets();
            for (auto company : gameState->getCompanies())
            {
                for (auto b : company->findBuildingsByType(world::BuildingType::Street))
                {
                    auto value = std::dynamic_pointer_cast<world::buildings::Street>(b);
                    streets.push_back(value);
                }
            }
            for (auto &street : streets)
            {
                foundPosition = false;
                auto pos = street->get2DPosition();
                auto northBuilding = gameState->getGameMap()->getBuilding2D({pos.x, pos.y - 1, pos.width, 1});
                auto southBuilding = gameState->getGameMap()->getBuilding2D({pos.x, pos.y + pos.height, 1, pos.height});
                auto eastBuilding = gameState->getGameMap()->getBuilding2D({pos.x + pos.width, pos.y, pos.width, 1});
                auto westBuilding = gameState->getGameMap()->getBuilding2D({pos.x - 1, pos.y, 1, pos.height});

                if (northBuilding == nullptr)
                {
                    targetBuilding->setPosition(pos.x, pos.y - 1);
                    foundPosition = true;
                }
                else if (southBuilding == nullptr)
                {
                    targetBuilding->setPosition(pos.x, pos.y + pos.height);
                    foundPosition = true;
                }
                else if (eastBuilding == nullptr)
                {
                    targetBuilding->setPosition(pos.x + pos.width, pos.y);
                    foundPosition = true;
                }
                else if (westBuilding == nullptr)
                {
                    targetBuilding->setPosition(pos.x - 1, pos.y);
                    foundPosition = true;
                }

                if (targetBuilding->canBuild(m_company->getCash()) && gameState->getGameMap()->canBuild(targetBuilding->get2DPosition()) && foundPosition)
                {
                    break;
                }
                else if (!gameState->getGameMap()->canBuild(targetBuilding->get2DPosition()))
                {
                    foundPosition = false;
                }
            }

            if (!foundPosition)
            {
                auto productionAction = std::make_shared<world::actions::BuildAction>(m_company, m_city, world::BuildingType::Street, m_product);
                setNextAction(productionAction);
            }

            if (targetBuilding->canBuild(m_company->getCash()) && gameState->getGameMap()->canBuild(targetBuilding->get2DPosition()) && foundPosition)
            {
                m_company->addBuilding(targetBuilding);
                gameState->getGameMap()->addBuilding(targetBuilding);

                m_company->incCash(targetBuilding->getBuildPrice() * -1);
                auto productionAction = std::make_shared<world::actions::ProductionAction>(m_company, targetBuilding, m_product);
                setNextAction(productionAction);
            }
        }
    }
}