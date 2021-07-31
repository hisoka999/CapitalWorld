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
class Product;

struct ProductRequirement
{
    std::shared_ptr<Product> product;
    int amount;
};

struct ResourceRequirement
{
    std::shared_ptr<Resource> resource;
    int amount;
};

class Product
{
public:
    Product(std::string name, std::string image, world::BuildingType buildingType, ProductionCycle cycle);
    std::string getName();
    std::string getImage();
    std::vector<std::shared_ptr<ResourceRequirement>> getResources();
    std::vector<std::shared_ptr<ProductRequirement>> getBaseProducts();

    void addRessource(std::shared_ptr<Resource> resource, int amount);
    bool needsResource(std::shared_ptr<Resource> resource);
    void addProduct(std::shared_ptr<Product> product, int amount);
    bool needsProduct(std::shared_ptr<Product> product);
    ProductionCycle getProductionCycle();

    float calculateCostsPerMonth();
    float calculateCostsPerPiece();
    world::BuildingType getBuildingType();

private:
    std::string name;
    std::string image;
    world::BuildingType buildingType;
    std::vector<std::shared_ptr<ResourceRequirement>> resources;
    std::vector<std::shared_ptr<ProductRequirement>> products;
    ProductionCycle cycle;
};

#endif // PRODUCT_H
