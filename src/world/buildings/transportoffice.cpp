#include "transportoffice.h"

namespace world
{
    namespace buildings
    {
        TransportOffice::TransportOffice(std::string name, std::string displayName, std::string description, int buildPirce, BuildingType type, int blockWidth, int blockHeight)
            : world::Building(name, displayName, description, buildPirce, type, blockWidth, blockHeight)
        {
        }
    }
}