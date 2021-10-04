#include "productservice.h"
#include "ressourceservice.h"
#include "magic_enum.hpp"
#include <algorithm>

namespace services
{

    std::vector<std::shared_ptr<Product>> ProductService::getProductsByBuildingType(world::BuildingType type)
    {
        std::vector<std::shared_ptr<Product>> result;
        for (auto &product : getData())
        {
            if (product->getBuildingType() == type)
            {
                result.push_back(product);
            }
        }
        return result;
    }
    std::vector<std::shared_ptr<Product>> ProductService::getBaseProductsByBuildingType(world::BuildingType type)
    {
        std::vector<std::shared_ptr<Product>> result;
        for (auto &product : getData())
        {
            if (product->getBuildingType() == type)
            {
                auto baseProducts = product->getBaseProducts();
                for (auto &baseProduct : baseProducts)
                {

                    //check if base product is in result list
                    bool isInList = false;
                    for (auto &resultProduct : result)
                    {
                        if (resultProduct->getName() == baseProduct->product->getName())
                        {
                            isInList = true;
                        }
                    }
                    if (!isInList)
                    {
                        result.push_back(baseProduct->product);
                    }
                }
            }
        }
        return result;
    }

    std::vector<std::shared_ptr<Product>> ProductService::getProductsByTypeAndResource(world::BuildingType type, std::shared_ptr<Resource> resource)
    {
        std::vector<std::shared_ptr<Product>> result;
        for (auto &product : getData())
        {
            if (product->getBuildingType() == type && product->needsResource(resource))
            {
                result.push_back(product);
            }
        }
        return result;
    }

    std::shared_ptr<Product> ProductService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
    {
        std::string name = object->getStringValue("name");
        std::string texture = object->getStringValue("texture");
        world::BuildingType type = magic_enum::enum_cast<world::BuildingType>(object->getStringValue("building")).value();
        world::ProductType productType = magic_enum::enum_cast<world::ProductType>(object->getStringValue("type")).value();

        ProductionCycle cycle = convertJsonObject2Cycle(object->getObjectValue("productionCycle"));
        auto product = std::make_shared<Product>(name, texture, type, cycle, productType);
        auto attrs = object->getAttributes();
        if (std::find(attrs.begin(), attrs.end(), std::string("baseResources")) != attrs.end())
        {
            auto baseResources = object->getObjectValue("baseResources");
            for (auto attr : baseResources->getAttributes())
            {
                product->addRessource(RessourceService::Instance().getResourceByName(attr), baseResources->getIntValue(attr));
            }
        }
        if (std::find(attrs.begin(), attrs.end(), std::string("baseProducts")) != attrs.end())
        {
            for (auto attr : object->getObjectValue("baseProducts")->getAttributes())
            {
                product->addProduct(getProductByName(attr), object->getObjectValue("baseProducts")->getIntValue(attr));
            }
        }

        return product;
    }

    ProductionCycle ProductService::convertJsonObject2Cycle(const std::shared_ptr<utils::JSON::Object> &object)
    {
        int start = object->getIntValue("start");
        int end = object->getIntValue("end");
        int time = object->getIntValue("time");
        int amount = object->getIntValue("amount");
        auto cycle = ProductionCycle(start, end, time, amount);
        return cycle;
    }

    std::shared_ptr<Product> ProductService::getProductByName(std::string name)
    {
        for (auto &product : getData())
        {
            if (product->getName() == name)
            {
                return product;
            }
        }
        return nullptr;
    }
}
