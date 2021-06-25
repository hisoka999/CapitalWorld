#include "product.h"

Product::Product(std::string name, std::string image, world::BuildingType buildingType, ProductionCycle cycle)
    : name(name), image(image), buildingType(buildingType), cycle(cycle)
{
}
std::string Product::getName()
{
    return name;
}
std::string Product::getImage()
{
    return image;
}
std::vector<std::shared_ptr<Resource>> Product::getResources()
{
    return resources;
}
std::vector<std::shared_ptr<ProductRequirement>> Product::getBaseProducts()
{
    return products;
}

void Product::addRessource(std::shared_ptr<Resource> resource)
{
    resources.push_back(resource);
}

world::BuildingType Product::getBuildingType()
{
    return buildingType;
}

float Product::calculateCostsPerMonth()
{
    return calculateCostsPerPiece() * cycle.amount;
}
float Product::calculateCostsPerPiece()
{
    float result = 0.0f;
    for (auto &res : resources)
    {
        result += res->getCostPerMonth();
    }
    for (auto &pro : products)
    {
        result += pro->product->calculateCostsPerPiece();
    }

    return result;
}
bool Product::needsResource(std::shared_ptr<Resource> resource)
{
    bool result = false;
    for (auto &res : resources)
    {
        if (res->getName() == resource->getName())
        {
            result = true;
            break;
        }
    }
    return result;
}

void Product::addProduct(std::shared_ptr<Product> product, int amount)
{
    ProductRequirement r = {product, amount};
    std::shared_ptr requirement = std::make_shared<ProductRequirement>(r);
    products.push_back(requirement);
}
bool Product::needsProduct(std::shared_ptr<Product> product)
{
    bool result = false;
    for (auto &res : products)
    {
        if (res->product->getName() == product->getName())
        {
            result = true;
            break;
        }
    }
    return result;
}

ProductionCycle Product::getProductionCycle()
{
    return cycle;
}
