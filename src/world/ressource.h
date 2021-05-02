#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <string>
#include "../world/buildingtypes.h"

class Resource
{
public:
    Resource(std::string name, std::string image, float costPerMonth, world::BuildingType buildingType);

    std::string getName();
    std::string getImage();
    float getCostPerMonth();
    world::BuildingType getBuildingType();

private:
    std::string name;
    std::string image;
    float costPerMonth;
    world::BuildingType buildingType;
};

#endif // RESSOURCE_H
