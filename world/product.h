#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>
#include <world/ressource.h>

class Product
{
public:
    Product(std::string name,std::string image,BuildingType buildingType);
    std::string getName();
    std::string getImage();
    std::vector<Resource> getResources();
    void addRessource(Resource resource);

    float calculateCostsPerMonth();
    BuildingType getBuildingType();

private:
    std::string name;
    std::string image;
    BuildingType buildingType;
    std::vector<Resource> resources;
};

#endif // PRODUCT_H
