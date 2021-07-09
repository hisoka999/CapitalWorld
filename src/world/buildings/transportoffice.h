#pragma once

#include <world/building.h>
#include <vector>

namespace world
{
    namespace buildings
    {
        struct TransportRoute
        {
            std::shared_ptr<world::Building> startBuilding;
            std::shared_ptr<world::Building> endBuilding;
            std::shared_ptr<Product> product;
            unsigned quantity;
            bool active;
        };

        class TransportOffice : public world::Building
        {
        public:
            TransportOffice(std::string name, std::string displayName, std::string description, int buildPirce, BuildingType type, int blockWidth, int blockHeight);
            TransportOffice(TransportOffice &copy);

            const std::vector<std::shared_ptr<TransportRoute>> &getAllRoutes();
            std::vector<std::shared_ptr<TransportRoute>> getActiveRoutes();
            void addRoute(const std::shared_ptr<world::Building> &startBuilding, const std::shared_ptr<world::Building> &endBuilding, const std::shared_ptr<Product> &product, const unsigned quantity);
            void updateProduction(int month, int year);

        private:
            std::vector<std::shared_ptr<TransportRoute>> routes;
        };
    };
};
