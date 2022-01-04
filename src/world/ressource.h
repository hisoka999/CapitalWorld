#ifndef RESSOURCE_H
#define RESSOURCE_H

#include "../world/buildingtypes.h"
#include <string>
#include "produceableobject.h"

class Resource : public ProduceableObject
{
public:
    Resource(const std::string &localizedName, const std::string &name, const std::string &image, float costPerMonth, world::BuildingType buildingType, world::RawResource rawResource);

    float getCostPerMonth();
    world::BuildingType getBuildingType();
    world::RawResource getRawResource();

private:
    float costPerMonth;
    world::BuildingType buildingType;
    world::RawResource rawResource;
};

#endif // RESSOURCE_H
