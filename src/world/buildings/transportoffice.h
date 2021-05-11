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
        };

        class TransportOffice : public world::Building
        {
        public:
            TransportOffice(std::string name, std::string displayName, std::string description, int buildPirce, BuildingType type, int blockWidth, int blockHeight);

        private:
            std::vector<TransportRoute> routes;
        };
    };
};
