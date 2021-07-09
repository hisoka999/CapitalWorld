#include "transportoffice.h"
#include "world/building.h"
namespace world
{
    namespace buildings
    {
        TransportOffice::TransportOffice(std::string name, std::string displayName, std::string description, int buildPirce, BuildingType type, int blockWidth, int blockHeight)
            : world::Building(name, displayName, description, buildPirce, type, blockWidth, blockHeight)
        {
        }

        TransportOffice::TransportOffice(TransportOffice &copy)
            : world::Building(copy.getName(), copy.getDisplayName(), copy.getDescription(), copy.getBuildPrice(), copy.getType(), copy.get2DPosition().width, copy.get2DPosition().height)
        {
            setSubTexture(copy.getSubTexture());
            setOffset(copy.getXOffset(), copy.getYOffset());
            setSourceRect(copy.getSourceRect());
        }

        const std::vector<std::shared_ptr<TransportRoute>> &TransportOffice::getAllRoutes()
        {
            return routes;
        }

        std::vector<std::shared_ptr<TransportRoute>> TransportOffice::getActiveRoutes()
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

        void TransportOffice::addRoute(const std::shared_ptr<world::Building> &startBuilding, const std::shared_ptr<world::Building> &endBuilding, const std::shared_ptr<Product> &product, const unsigned quantity)
        {
            TransportRoute tmp = {startBuilding, endBuilding, product, quantity, false};
            std::shared_ptr<TransportRoute> route = std::make_shared<TransportRoute>(tmp);
            routes.push_back(route);
        }

        void TransportOffice::updateProduction(int month, int year)
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
    }
}