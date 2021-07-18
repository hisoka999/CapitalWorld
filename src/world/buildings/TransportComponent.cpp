#include "TransportComponent.h"
#include "world/building.h"
#include "world/product.h"
#include "services/productservice.h"
#include "services/buildingservice.h"
#include "world/company.h"

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

        void TransportComponent::fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company)
        {
            utils::JSON::JsonArray jsonRoutes = object->getArray("routes");
            for (auto &jsonRoute : jsonRoutes)
            {
                auto routeObject = std::get<std::shared_ptr<utils::JSON::Object>>(jsonRoute);
                std::shared_ptr<TransportRoute> route = std::make_shared<TransportRoute>();
                route->active = routeObject->getBoolValue("active");
                route->product = services::ProductService::Instance().getProductByName(routeObject->getStringValue("product"));
                route->startBuilding = company->findBuildingByDisplayName(routeObject->getStringValue("startBuilding"));
                route->endBuilding = company->findBuildingByDisplayName(routeObject->getStringValue("endBuilding"));
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
            TransportRoute tmp = {startBuilding, endBuilding, product, quantity, false};
            std::shared_ptr<TransportRoute> route = std::make_shared<TransportRoute>(tmp);
            routes.push_back(route);
        }

        void TransportComponent::updateProduction(int month, int year)
        {
            for (auto &route : routes)
            {
                if (!route->active || route->product == nullptr || route->endBuilding == nullptr)
                    continue;
                auto &startStorage = route->startBuilding->getStorage();
                auto &endStorage = route->endBuilding->getStorage();
                int amount = startStorage.getEntry(route->product->getName());
                startStorage.addEntry(route->product->getName(), amount * -1);
                endStorage.addEntry(route->product->getName(), amount);

                route->endBuilding->addCosts(month, year, route->product->getName(), world::BalanceAccount::Transport, amount * 0.1);
            }
        }

        std::shared_ptr<BuildingComponent> TransportComponent::clone()
        {
            return std::make_shared<TransportComponent>();
        }
    }
}