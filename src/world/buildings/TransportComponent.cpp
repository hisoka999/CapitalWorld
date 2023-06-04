#include "TransportComponent.h"
#include "services/buildingservice.h"
#include "services/productservice.h"
#include "world/building.h"
#include "world/buildings/StorageComponent.h"
#include "world/company.h"
#include "world/product.h"
#include <future>
#include "world/gamestate.h"
#include "world/GameStateMutex.h"
#include "messages.h"
#include "world/AnimatedMovement.h"
#include <engine/graphics/TextureManager.h>
#include <magic_enum.hpp>
#include "WorkerComponent.h"

namespace world
{
    namespace buildings
    {
        TransportComponent::TransportComponent() : world::buildings::BuildingComponent("TransportComponent")
        {
        }

        TransportComponent::~TransportComponent()
        {
        }

        std::shared_ptr<utils::JSON::Object> TransportComponent::toJson()
        {
            auto object = world::buildings::BuildingComponent::toJson();

            utils::JSON::JsonArray jsonRoutes;
            for (auto route : routes)
            {
                std::shared_ptr<utils::JSON::Object> routeObject = std::make_shared<utils::JSON::Object>();

                routeObject->setAttribute("active", route->active);
                routeObject->setAttribute("product", route->product->getName());
                routeObject->setAttribute("quantity", int(route->quantity));
                routeObject->setAttribute("startBuilding", route->startBuilding->getDisplayName());
                routeObject->setAttribute("endBuilding", route->endBuilding->getDisplayName());

                utils::JSON::JsonValue value = routeObject;
                jsonRoutes.push_back(value);
            }
            object->setArrayAttribute("routes", jsonRoutes);

            return object;
        }

        void TransportComponent::fromJson(std::shared_ptr<utils::JSON::Object> &object, [[maybe_unused]] Company *company)
        {
            utils::JSON::JsonArray jsonRoutes = object->getArray("routes");
            for (auto &jsonRoute : jsonRoutes)
            {
                auto routeObject = std::get<std::shared_ptr<utils::JSON::Object>>(jsonRoute);
                std::shared_ptr<TransportRoute> route = std::make_shared<TransportRoute>();
                route->active = routeObject->getBoolValue("active");

                route->startBuildingName = routeObject->getStringValue("startBuilding");
                route->endBuildingName = routeObject->getStringValue("endBuilding");
                route->product = services::ProductService::Instance().getProductByName(routeObject->getStringValue("product"));

                routes.push_back(route);
            }
        }

        const std::vector<std::shared_ptr<TransportRoute>> &TransportComponent::getAllRoutes()
        {
            return routes;
        }

        std::vector<std::shared_ptr<TransportRoute>> TransportComponent::getActiveRoutes()
        {
            std::vector<std::shared_ptr<TransportRoute>> result;
            for (auto route : routes)
            {
                if (route->active)
                {
                    result.push_back(route);
                }
            }
            return result;
        }

        void TransportComponent::clearRoutes()
        {
            std::lock_guard<std::mutex> guard(gGameStateMutex);

            routes.clear();
        }

        void TransportComponent::addRoute(const std::shared_ptr<world::Building> &startBuilding, const std::shared_ptr<world::Building> &endBuilding, const std::shared_ptr<Product> &product, const unsigned quantity)
        {
            if (routes.size() == MAX_ROUTES)
                return;
            TransportRoute tmp;
            tmp.startBuilding = startBuilding;
            tmp.endBuilding = endBuilding;
            if (startBuilding != nullptr)
            {
                tmp.startBuildingName = startBuilding->getDisplayName();
            }

            if (endBuilding != nullptr)
            {
                tmp.endBuildingName = endBuilding->getDisplayName();
            }
            tmp.product = product;
            tmp.quantity = quantity;
            tmp.active = false;
            std::shared_ptr<TransportRoute> route = std::make_shared<TransportRoute>(tmp);
            auto is_equal = [&](std::shared_ptr<TransportRoute> &r)
            {
                return r->endBuildingName == route->endBuildingName && r->startBuildingName == route->startBuildingName && r->product == route->product;
            };

            if (std::find_if(std::begin(routes), std::end(routes), is_equal) == std::end(routes))
            {
                routes.push_back(route);
            }
        }

        void TransportComponent::removeRoute(const size_t position)
        {
            auto it = routes.begin() + position;

            if (it != std::end(routes))
                routes.erase(it);
        }

        void TransportComponent::updateProduction([[maybe_unused]] int month, [[maybe_unused]] int year, [[maybe_unused]] Building *building)
        {
        }

        void TransportComponent::updateDaily([[maybe_unused]] uint16_t day, uint16_t month, uint16_t year, [[maybe_unused]] Building *building, Company *company)
        {
            std::lock_guard<std::mutex> guard(gGameStateMutex);

            auto workerComp = building->getComponent<WorkerComponent>("WorkerComponent");

            int numberOfActiveRoutes = 0;

            for (auto &route : routes)
            {

                if (route == nullptr)
                    continue;
                if (!route->active || route->product == nullptr || route->endBuilding == nullptr || route->startBuilding == nullptr)
                    continue;

                if (!route->transportActive)
                    continue;

                if (!route->transportFinished)
                {
                    numberOfActiveRoutes++;
                    continue;
                }
                numberOfActiveRoutes--;
                auto startStorage = route->startBuilding->getComponent<world::buildings::StorageComponent>("StorageComponent");
                auto endStorage = route->endBuilding->getComponent<world::buildings::StorageComponent>("StorageComponent");
                int amount = route->quantity;
                if (endStorage->canAdd(route->product->getName(), amount))
                {
                    endStorage->addEntry(route->product->getName(), amount);

                    route->endBuilding->getBalance().addCosts(month, year, route->product->getName(), world::BalanceAccount::Transport, amount * 0.1);
                }
                route->transportActive = false;
                route->transportFinished = false;
            }

            for (auto &route : routes)
            {
                if (route == nullptr)
                    continue;
                if (!route->active || route->product == nullptr || route->endBuilding == nullptr || route->startBuilding == nullptr)
                    continue;

                if (route->transportActive)
                    continue;
                auto startStorage = route->startBuilding->getComponent<world::buildings::StorageComponent>("StorageComponent");
                int amount = startStorage->getEntry(route->product->getName());
                if (amount == 0)
                    continue;
                // find path
                const auto &graph = getGameState()->getGameMap()->getStreetGraph();
                auto startStreets = getGameState()->getGameMap()->borderingBuilding(route->startBuilding, world::BuildingType::Street, false);
                auto endStreets = getGameState()->getGameMap()->borderingBuilding(route->endBuilding, world::BuildingType::Street, false);

                if (startStreets.size() == 0 || endStreets.size() == 0)
                    continue;
                auto startPosition = startStreets[0]->getPosition();

                size_t targetIndex = getGameState()->getGameMap()->getGraphIndex(endStreets[0]->getPosition());
                if (targetIndex != 0 && workerComp->getCurrentWorkers() > numberOfActiveRoutes)
                {
                    if (route->path.size() == 0)
                    {
                        size_t startIndex = getGameState()->getGameMap()->getGraphIndex(startPosition);
                        std::vector<double> minimumDistances;
                        std::vector<int> previousVertices;
                        std::map<int, utils::Vector2> vertexPositions;
                        vertexPositions[startIndex] = startStreets[0]->getPosition();

                        paths::ComputeShortestPathsByDijkstra(startIndex, graph, minimumDistances, previousVertices, vertexPositions);

                        if (paths::maximumWeight != minimumDistances[targetIndex])
                        {
                            route->path = paths::GetShortestPathTo(targetIndex, previousVertices, vertexPositions);
                            assert(route->path.front().getX() != 0.0);
                        }
                        else
                            route->path.clear();
                    }

                    if (route->path.size() > 1)
                    {
                        route->quantity = std::min(amount, int(route->maxQuantity));
                        startStorage->addEntry(route->product->getName(), route->quantity * -1);
                        route->transportActive = true;
                        AnimatedMovementData data = {route, std::string(magic_enum::enum_name(company->getColor()))};
                        std::shared_ptr<core::Message<MessageTypes, AnimatedMovementData>> message = std::make_shared<core::Message<MessageTypes, AnimatedMovementData>>(MessageTypes::AnimationStart, data);
                        core::MessageSystem<MessageTypes>::get().sendMessage(message);
                    }
                    else if (route->path.size() == 1)
                    {
                        route->transportActive = true;
                    }
                    numberOfActiveRoutes++;
                    return;
                }
            }
        }

        std::shared_ptr<BuildingComponent> TransportComponent::clone()
        {
            return std::make_shared<TransportComponent>();
        }

        void TransportComponent::delayedUpdate(Company *company)
        {
            for (auto &route : routes)
            {
                route->startBuilding = company->findBuildingByDisplayName(route->startBuildingName);
                route->endBuilding = company->findBuildingByDisplayName(route->endBuildingName);
            }
        }
    }
}