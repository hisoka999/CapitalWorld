#include "TransportComponent.h"
#include "services/buildingservice.h"
#include "services/productservice.h"
#include "world/building.h"
#include "world/buildings/StorageComponent.h"
#include "world/company.h"
#include "world/product.h"
#include <future>

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

        void TransportComponent::addRoute(const std::shared_ptr<world::Building> &startBuilding, const std::shared_ptr<world::Building> &endBuilding, const std::shared_ptr<Product> &product, const unsigned quantity)
        {
            TransportRoute tmp = {startBuilding, endBuilding, "", "", product, quantity, false};
            std::shared_ptr<TransportRoute> route = std::make_shared<TransportRoute>(tmp);
            routes.push_back(route);
        }

        void TransportComponent::removeRoute(const size_t position)
        {
            auto it = routes.begin() + position;

            if (it != std::end(routes))
                routes.erase(it);
        }

        void TransportComponent::updateProduction(int month, int year, [[maybe_unused]] Building *building)
        {
            for (auto &route : routes)
            {
                if (!route->active || route->product == nullptr || route->endBuilding == nullptr || route->startBuilding == nullptr)
                    continue;

                auto startStorage = route->startBuilding->getComponent<world::buildings::StorageComponent>("StorageComponent");
                auto endStorage = route->endBuilding->getComponent<world::buildings::StorageComponent>("StorageComponent");
                int amount = startStorage->getEntry(route->product->getName());
                if (endStorage->canAdd(route->product->getName(), amount))
                {
                    startStorage->addEntry(route->product->getName(), amount * -1);
                    endStorage->addEntry(route->product->getName(), amount);

                    route->endBuilding->addCosts(month, year, route->product->getName(), world::BalanceAccount::Transport, amount * 0.1);
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