#include "productservice.h"

namespace services {


ProductService* ProductService::instance = nullptr;
std::once_flag ProductService::onceFlag;
std::vector<Product> ProductService::getProductsByBuildingType(BuildingType type)
{
    std::vector<Product> result;
    for(auto& product : products)
    {
        if(product.getBuildingType() == type)
        {
            result.push_back(product);
        }
    }
    return result;
}
std::vector<Resource> ProductService::getResourcesByBuildingType(BuildingType type)
{
    std::vector<Resource> result;
    for (auto& res : resources)
    {
        if(res.getBuildingType() == type)
        {
            result.push_back(res);
        }
    }
    return result;
}
void ProductService::loadProducts(std::string path)
{
    Product eggs("Eggs","",BuildingType::Farm);
    eggs.addRessource(*getResourceByName("Chicken"));
    products.push_back(eggs);

    Product wheet("Wheet","",BuildingType::Farm);
    wheet.addRessource(*getResourceByName("Wheet"));
    products.push_back(wheet);


}
void ProductService::loadResources(std::string path)
{
    Resource chicken("Chicken","",20,BuildingType::Farm);
    resources.push_back(chicken);

    Resource wheet("Wheet","",10,BuildingType::Farm);
    resources.push_back(wheet);
}

Resource* ProductService::getResourceByName(std::string name)
{
    for (auto& res : resources)
    {
        if(res.getName() == name)
        {
            return &res;
        }
    }
    return nullptr;

}
Product* ProductService::getProductByName(std::string name)
{
    for(auto& product : products)
    {
        if(product.getName() == name)
        {
            return &product;
        }
    }
    return nullptr;
}

}
