#ifndef RESSOURCE_H
#define RESSOURCE_H

#include "../world/buildingtypes.h"
#include <string>

class Resource
{
public:
    Resource(std::string name, std::string image, float costPerMonth, world::BuildingType buildingType, world::RawResource rawResource);

    std::string getName();
    std::string getImage();
    float getCostPerMonth();
    world::BuildingType getBuildingType();
    world::RawResource getRawResource();

private:
    std::string name;
    std::string image;
    float costPerMonth;
    world::BuildingType buildingType;
    world::RawResource rawResource;
};

#endif // RESSOURCE_H
