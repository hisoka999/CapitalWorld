#include "ProductionAction.h"
#include <services/ressourceservice.h>
#include <services/productservice.h>
#include <random>
#include <algorithm>
#include <world/buildings/SalesComponent.h>
#include <world/buildings/StorageComponent.h>
#include <world/buildings/TransportComponent.h>
#include <world/buildings/WorkerComponent.h>
#include <world/actions/BuildAction.h>

namespace world
{
    namespace actions
    {
        ProductionAction::ProductionAction(const std::shared_ptr<world::Company> &company, const std::shared_ptr<world::Building> &building, const std::shared_ptr<Product> &product)
            : Action(company), m_building(building), m_product(product)
        {
        }

        void ProductionAction::execute([[maybe_unused]] const std::shared_ptr<world::GameState> &gameState)
        {

            switch (m_building->getType())
            {
            case world::BuildingType::Resource:
            {
                if (m_product != nullptr)
                {
                    m_building->addProduct(m_product); // TODO is that all?
                }
                else
                {
                    auto position2D = m_building->get2DPosition().toVecto2();
                    // TileType tile = gameState->getGameMap()->getTile(position2D);
                    world::RawResource rawResource = gameState->getGameMap()->getRawResource(position2D.getX(), position2D.getY());
                    if (m_building->requireResource(rawResource))
                    {
                        auto resourceList = services::ResourceService::Instance().getResourcesByBuildingType(world::BuildingType::Resource, rawResource);
                    }
                }

                auto workers = m_building->getComponent<world::buildings::WorkerComponent>("WorkerComponent");
                workers->setCurrentWorkers(m_building->getProducts().size());
            }
            break;
            case world::BuildingType::Factory:
            {
                if (m_product != nullptr)
                {
                    m_building->addProduct(m_product);
                    auto workers = m_building->getComponent<world::buildings::WorkerComponent>("WorkerComponent");
                    workers->setCurrentWorkers(m_building->getProducts().size());

                    for (auto base : m_product->getBaseProducts())
                    {
                        switch (base->product->getBuildingType())
                        {
                        case world::BuildingType::Factory:
                            m_building->addProduct(base->product);
                            {
                                workers->setCurrentWorkers(m_building->getProducts().size());
                            }
                            /* code */
                            break;
                        case world::BuildingType::Resource:
                        {
                            // todo find available fitting building as an alternative
                            bool foundProduction = false;
                            for (auto &productionBuilding : m_company->findBuildingsByType(world::BuildingType::Resource))
                            {
                                for (auto res : base->product->getResources())
                                {
                                    if (productionBuilding->requireResource(res->resource->getRawResource()))
                                    {
                                        productionBuilding->addProduct(base->product);
                                        foundProduction = true;

                                        auto workers = productionBuilding->getComponent<world::buildings::WorkerComponent>("WorkerComponent");
                                        workers->setCurrentWorkers(productionBuilding->getProducts().size());
                                    }
                                }
                            }
                            if (!foundProduction)
                            {
                                auto targetCity = findTargetCity(gameState, m_building);
                                auto resourceBuilding = std::make_shared<BuildAction>(m_company, targetCity, world::BuildingType::Resource, base->product);
                                setNextAction(resourceBuilding);
                            }
                            break;
                        }
                        default:
                            break;
                        }
                    }
                }
            }
            break;
            case world::BuildingType::Transport:
            {
                const auto &transport = m_building->getComponent<world::buildings::TransportComponent>("TransportComponent");
                transport->clearRoutes();

                for (auto &building : m_company->getBuildings())
                {
                    auto possibleBuildings = gameState->getGameMap()->findStorageBuildings(building, m_company);

                    if (building->getType() == BuildingType::Shop)
                    {
                        const auto &sales = building->getComponent<world::buildings::SalesComponent>("SalesComponent");
                        for (auto &item : sales->getSalesItems())
                        {

                            for (auto production : possibleBuildings)
                            {
                                if (production->getType() == BuildingType::Shop)
                                {
                                    continue;
                                }
                                for (auto product : production->getProducts())
                                {
                                    if (product->getName() == item->product)
                                    {
                                        transport->addRoute(production, building, product, 100);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        for (auto &product : building->getProducts())
                        {
                            for (auto &base : product->getBaseProducts())
                            {

                                for (auto &startBuilding : possibleBuildings)
                                {
                                    if (startBuilding->hasProduct(base->product) && startBuilding->getDisplayName() != building->getDisplayName())
                                    {

                                        transport->addRoute(startBuilding, building, base->product, base->amount);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                for (auto &route : transport->getAllRoutes())
                {

                    route->active = true;
                }
                transport->delayedUpdate(m_company.get());
                auto workers = m_building->getComponent<world::buildings::WorkerComponent>("WorkerComponent");
                workers->setCurrentWorkers(transport->getActiveRoutes().size());
                break;
            }
            case world::BuildingType::Shop:
                // find product to be sold
                {
                    auto products = m_company->findAvailableProducts();
                    for (auto it = products.begin(); it != products.end(); it++)
                    {
                        if (m_building->hasProduct(*it))
                        {
                            products.erase(it);
                        }
                    }
                    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
                    std::shuffle(products.begin(), products.end(), gen);
                    if (products.size() > 0)
                    {
                        auto product = products[0];
                        auto sales = m_building->getComponent<world::buildings::SalesComponent>("SalesComponent");
                        sales->setGameMap(gameState->getGameMap().get());
                        sales->addSalesItem(product->getName(), product->calculateCostsPerPiece() * 1.5, true);

                        auto storage = m_building->getComponent<world::buildings::StorageComponent>("StorageComponent");
                        storage->addEntry(product->getName(), 1);
                        storage->addEntry(product->getName(), -1);
                        // switch (product->getBuildingType())
                        // {
                        // case world::BuildingType::Factory:
                        // }
                        auto targetCity = findTargetCity(gameState, m_building);

                        for (auto &prodBuilding : m_company->findBuildingsByType(product->getBuildingType()))
                        {
                            auto resourceBuilding = std::make_shared<ProductionAction>(m_company, prodBuilding, product);
                            setNextAction(resourceBuilding);
                            return;
                        }

                        const auto buildAction = std::make_shared<world::actions::BuildAction>(m_company, targetCity, product->getBuildingType(), product);
                        setNextAction(buildAction);
                    }
                }
                break;
            default:
                break;
            }
        }
        bool ProductionAction::canExecute([[maybe_unused]] const std::shared_ptr<world::GameState> &gameState)
        {

            switch (m_building->getType())
            {
            case BuildingType::Transport:
            {
                const auto &transport = m_building->getComponent<world::buildings::TransportComponent>("TransportComponent");
                if (transport->getAllRoutes().size() == world::buildings::MAX_ROUTES)
                {
                    return false;
                }

                break;
            }
            case BuildingType::Shop:
            {
                auto sales = m_building->getComponent<world::buildings::SalesComponent>("SalesComponent");
                if (sales->getSalesItems().size() == world::buildings::MAX_SALES_ITEMS)
                {
                    return false;
                }
            }
            default:
                break;
            }
            return true;
        }
    }
}