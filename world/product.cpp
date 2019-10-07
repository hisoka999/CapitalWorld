#include "product.h"

Product::Product(std::string name,std::string image,BuildingType buildingType)
    :name(name),image(image),buildingType(buildingType)
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
std::vector<Resource> Product::getResources()
{
    return resources;
}
void Product::addRessource(Resource resource)
{
    resources.push_back(resource);
}

BuildingType Product::getBuildingType()
{
    return buildingType;
}

float Product::calculateCostsPerMonth()
{
    float result = 0.0f;
    for(auto& res : resources)
    {
        result+= res.getCostPerMonth();
    }
    return result;
}
