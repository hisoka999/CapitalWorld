#include "ressource.h"

Resource::Resource(std::string name, std::string image, float costPerMonth, world::BuildingType buildingType, world::RawResource rawResource)
    : name(name), image(image), costPerMonth(costPerMonth), buildingType(buildingType), rawResource(rawResource)
{
}

std::string Resource::getName()
{
    return name;
}
std::string Resource::getImage()
{
    return image;
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
