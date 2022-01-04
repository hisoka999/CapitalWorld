#include "ressource.h"

Resource::Resource(const std::string &localizedName, const std::string &name, const std::string &image, float costPerMonth, world::BuildingType buildingType, world::RawResource rawResource)
    : ProduceableObject(localizedName, name, image), costPerMonth(costPerMonth), buildingType(buildingType), rawResource(rawResource)
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
