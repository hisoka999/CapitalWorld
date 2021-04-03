#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>
#include <memory>
#include "../world/ressource.h"

struct ProductionCycle
{
    unsigned startMonth;
    unsigned endMonth;
    //production time in days
    unsigned productionTime;
    unsigned amount;

    ProductionCycle(unsigned start, unsigned end, unsigned time, unsigned amount)
        : startMonth(start), endMonth(end), productionTime(time), amount(amount)
    {
    }
};

class Product
{
public:
    Product(std::string name, std::string image, BuildingType buildingType, ProductionCycle cycle);
    std::string getName();
    std::string getImage();
    std::vector<std::shared_ptr<Resource>> getResources();
    std::vector<std::shared_ptr<Product>> getBaseProducts();

    void addRessource(std::shared_ptr<Resource> resource);
    bool needsResource(std::shared_ptr<Resource> resource);
    void addProduct(std::shared_ptr<Product> product);
    bool needsProduct(std::shared_ptr<Product> product);
    ProductionCycle getProductionCycle();

    float calculateCostsPerMonth();
    float calculateCostsPerPiece();
    BuildingType getBuildingType();

private:
    std::string name;
    std::string image;
    BuildingType buildingType;
    std::vector<std::shared_ptr<Resource>> resources;
    std::vector<std::shared_ptr<Product>> products;
    ProductionCycle cycle;
};

#endif // PRODUCT_H
