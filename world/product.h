#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>
#include <memory>
#include <world/ressource.h>

struct ProductionCycle{
    unsigned startMonth;
    unsigned endMonth;
    //production time in days
    unsigned productionTime;

    ProductionCycle(unsigned start,unsigned end,unsigned time)
        :startMonth(start),endMonth(end),productionTime(time)
    {

    }
};

class Product
{
public:
    Product(std::string name,std::string image,BuildingType buildingType,ProductionCycle cycle);
    std::string getName();
    std::string getImage();
    std::vector<std::shared_ptr<Resource>> getResources();
    void addRessource(std::shared_ptr<Resource> resource);
    bool needsResource(std::shared_ptr<Resource> resource);
    ProductionCycle getProductionCycle();

    float calculateCostsPerMonth();
    BuildingType getBuildingType();

private:
    std::string name;
    std::string image;
    BuildingType buildingType;
    std::vector<std::shared_ptr<Resource>> resources;
    ProductionCycle cycle;
};

#endif // PRODUCT_H
