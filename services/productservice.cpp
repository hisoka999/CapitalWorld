#include "productservice.h"

namespace services {


ProductService* ProductService::instance = nullptr;
std::once_flag ProductService::onceFlag;
std::vector<std::shared_ptr<Product>> ProductService::getProductsByBuildingType(BuildingType type)
{
    std::vector<std::shared_ptr<Product>> result;
    for(auto& product : products)
    {
        if(product->getBuildingType() == type)
        {
            result.push_back(product);
        }
    }
    return result;
}
std::vector<std::shared_ptr<Product>> ProductService::getBaseProductsByBuildingType(BuildingType type)
{
    std::vector<std::shared_ptr<Product>> result;
    for(auto& product : products)
    {
        if(product->getBuildingType() == type)
        {
            auto baseProducts = product->getBaseProducts();
            for(auto& baseProduct : baseProducts)
            {

                //check if base product is in result list
                bool isInList = false;
                for(auto& resultProduct: result)
                {
                    if(resultProduct->getName() == baseProduct->getName())
                    {
                        isInList= true;
                    }
                }
                if(!isInList)
                {
                    result.push_back(baseProduct);
                }
            }
        }
    }
    return result;
}


std::vector<std::shared_ptr<Product>> ProductService::getProductsByTypeAndResource(BuildingType type, std::shared_ptr<Resource> resource)
{
    std::vector<std::shared_ptr<Product>> result;
    for(auto& product : products)
    {
        if(product->getBuildingType() == type
                && product->needsResource(resource))
        {
            result.push_back(product);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Resource>> ProductService::getResourcesByBuildingType(BuildingType type)
{
    std::vector<std::shared_ptr<Resource>> result;
    for (auto& res : resources)
    {
        if(res->getBuildingType() == type)
        {
            result.push_back(res);
        }
    }

    return result;
}
void ProductService::loadProducts(std::string path)
{
    ProductionCycle eggCycle(1,12,3*30,100);

    auto eggs = std::make_shared<Product>("Eggs","egg.png",BuildingType::Farm,eggCycle);
    eggs->addRessource(getResourceByName("Chicken"));
    products.push_back(eggs);

    auto wheet = std::make_shared<Product>("Wheet","wheat.png",BuildingType::Farm,ProductionCycle(3,7,30,300));
    wheet->addRessource(getResourceByName("Wheat"));
    products.push_back(wheet);

    auto bread = std::make_shared<Product>("Bread","bread.png",BuildingType::Factory,ProductionCycle(1,12,2,100));
    bread->addProduct(wheet);
    bread->addProduct(eggs);
    products.push_back(bread);

    auto beer = std::make_shared<Product>("Beer","beer.png",BuildingType::Factory,ProductionCycle(1,12,2,100));
    beer->addProduct(wheet);
    products.push_back(beer);
}
void ProductService::loadResources(std::string path)
{
    auto chicken = std::make_shared<Resource>("Chicken","chicken.png",20,BuildingType::Farm);
    resources.push_back(chicken);

    auto wheet = std::make_shared<Resource>("Wheat","wheat.png",10,BuildingType::Farm);
    resources.push_back(wheet);
}

std::shared_ptr<Resource> ProductService::getResourceByName(std::string name)
{
    for (auto& res : resources)
    {
        if(res->getName() == name)
        {
            return res;
        }
    }
    return nullptr;

}
std::shared_ptr<Product> ProductService::getProductByName(std::string name)
{
    for(auto& product : products)
    {
        if(product->getName() == name)
        {
            return product;
        }
    }
    return nullptr;
}

}
