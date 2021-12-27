#include "ressource.h"

Resource::Resource(std::string name, std::string image, float costPerMonth, world::BuildingType buildingType, world::RawResource rawResource)
    : ProduceableObject(name, image), costPerMonth(costPerMonth), buildingType(buildingType), rawResource(rawResource)
{
}

float Resource::getCostPerMonth()
{
    return costPerMonth;
}
world::BuildingType Resource::getBuildingType()
{
    return buildingType;
}

world::RawResource Resource::getRawResource()
{
    return rawResource;
}
