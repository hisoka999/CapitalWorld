#include "ProductionAction.h"
#include <services/ressourceservice.h>
#include <services/productservice.h>
#include <random>
#include <algorithm>
#include <world/buildings/SalesComponent.h>
#include <world/buildings/StorageComponent.h>
#include <world/buildings/TransportComponent.h>
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
            }
            break;
            case world::BuildingType::Factory:
            {
                if (m_product != nullptr)
                {
                    m_building->addProduct(m_product);
                    for (auto base : m_product->getBaseProducts())
                    {
                        switch (base->product->getBuildingType())
                        {
                        case world::BuildingType::Factory:
                            m_building->addProduct(base->product);
                            /* code */
                            break;
                        case world::BuildingType::Resource:
                            // TODO
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

                for (auto &building : m_company->getBuildings())
                {
                    if (building->getType() == BuildingType::Shop)
                    {
                        const auto &sales = building->getComponent<world::buildings::SalesComponent>("SalesComponent");
                        for (auto &item : sales->getSalesItems())
                        {

                            for (auto production : m_company->findProductionBuildings())
                            {
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

                                for (auto &startBuilding : m_company->getBuildings())
                                {
                                    if (startBuilding->hasProduct(base->product))
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
                        sales->addSalesItem(product->getName(), product->calculateCostsPerPiece() * 0.4, true);

                        auto storage = m_building->getComponent<world::buildings::StorageComponent>("StorageComponent");
                        storage->addEntry(product->getName(), 1);
                        storage->addEntry(product->getName(), -1);
                        // switch (product->getBuildingType())
                        // {
                        // case world::BuildingType::Factory:
                        // }
                        std::shared_ptr<City> targetCity = nullptr;
                        float distance = MAXFLOAT;
                        for (auto &city : gameState->getCities())
                        {
                            float currentDistance = city->getPosition().distance(m_building->get2DPosition().toVecto2());
                            if (currentDistance < distance)
                            {
                                distance = currentDistance;
                                targetCity = city;
                            }
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
    }
}