#include "product.h"

Product::Product(const std::string &localizedName, std::string name, std::string image, world::BuildingType buildingType, ProductionCycle cycle, world::ProductType type)
    : ProduceableObject(localizedName, name, image), buildingType(buildingType), cycle(cycle), productType(type)
{
}

std::vector<std::shared_ptr<ResourceRequirement>> &Product::getResources()
{
    return resources;
}
std::vector<std::shared_ptr<ProductRequirement>> &Product::getBaseProducts()
{
    return products;
}

void Product::addResource(std::shared_ptr<Resource> resource, int amount)
{
    std::shared_ptr<ResourceRequirement> requirement = std::make_shared<ResourceRequirement>();
    requirement->amount = amount;
    requirement->resource = resource;
    resources.push_back(requirement);
}

world::BuildingType Product::getBuildingType()
{
    return buildingType;
}

world::ProductType Product::getProductType()
{
    return productType;
}

double Product::calculateCostsPerMonth()
{
    return calculateCostsPerPiece() * cycle.amount;
}
double Product::calculateCostsPerPiece()
{
    double result = 0.0f;
    for (auto &res : resources)
    {
        result += (res->resource->getCostPerMonth() * float(res->amount)) / this->cycle.amount;
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
        if (res->resource->getName() == resource->getName())
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

ProductionCycle &Product::getProductionCycle()
{
    return cycle;
}
