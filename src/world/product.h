#ifndef PRODUCT_H
#define PRODUCT_H

#include "../world/ressource.h"
#include <memory>
#include <string>
#include <vector>
#include "produceableobject.h"

struct ProductionCycle
{
    unsigned startMonth;
    unsigned endMonth;
    // production time in days
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

class Product : public ProduceableObject
{
public:
    Product(const std::string &localizedName, std::string name, std::string image, world::BuildingType buildingType, ProductionCycle cycle, world::ProductType type);

    std::vector<std::shared_ptr<ResourceRequirement>> getResources();
    std::vector<std::shared_ptr<ProductRequirement>> getBaseProducts();

    void addResource(std::shared_ptr<Resource> resource, int amount);
    bool needsResource(std::shared_ptr<Resource> resource);
    void addProduct(std::shared_ptr<Product> product, int amount);
    bool needsProduct(std::shared_ptr<Product> product);
    ProductionCycle getProductionCycle();

    float calculateCostsPerMonth();
    float calculateCostsPerPiece();
    world::BuildingType getBuildingType();
    world::ProductType getProductType();

private:
    world::BuildingType buildingType;
    std::vector<std::shared_ptr<ResourceRequirement>> resources;
    std::vector<std::shared_ptr<ProductRequirement>> products;
    ProductionCycle cycle;
    world::ProductType productType;
};

#endif // PRODUCT_H
