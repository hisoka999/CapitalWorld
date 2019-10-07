#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <string>
#include <world/buildingtypes.h>

class Resource
{
public:
    Resource(std::string name,std::string image, float costPerMonth,BuildingType buildingType);

    std::string getName();
    std::string getImage();
    float getCostPerMonth();
    BuildingType getBuildingType();

private:
    std::string name;
    std::string image;
    float costPerMonth;
    BuildingType buildingType;

};

#endif // RESSOURCE_H
