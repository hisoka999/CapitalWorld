#include "product.h"

Product::Product(std::string name,std::string image,BuildingType buildingType,ProductionCycle cycle)
    :name(name),image(image),buildingType(buildingType),cycle(cycle)
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
void Product::addRessource(std::shared_ptr<Resource> resource)
{
    resources.push_back(resource);
}

BuildingType Product::getBuildingType()
{
    return buildingType;
}

float Product::calculateCostsPerMonth()
{
    return calculateCostsPerPiece()*cycle.amount;
}
float Product::calculateCostsPerPiece()
{
    float result = 0.0f;
    for(auto& res : resources)
    {
        result+= res->getCostPerMonth();
    }
    return result;
}
bool Product::needsResource(std::shared_ptr<Resource> resource)
{
    bool result = false;
    for(auto& res : resources)
    {
        if(res->getName() == resource->getName())
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
