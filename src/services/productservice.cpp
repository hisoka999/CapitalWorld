#include "productservice.h"
#include "magic_enum.hpp"
#include "ressourceservice.h"
#include <algorithm>
#include <engine/utils/localisation.h>

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

    std::vector<std::shared_ptr<Product>> ProductService::getProductsByBuildingType(world::BuildingType type, const std::shared_ptr<Product> &baseProduct)
    {
        std::vector<std::shared_ptr<Product>> result;
        for (auto &product : getData())
        {
            if (product->getBuildingType() == type)
            {
                for (auto &base : product->getBaseProducts())
                {
                    if (base->product->getName() == baseProduct->getName())
                    {
                        result.push_back(product);
                    }
                }
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

                    // check if base product is in result list
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

        std::string lang = Localisation::Instance().getLanguage();
        if (lang == "en")
            lang = "";
        else
        {
            lang = "_" + lang;
        }
        std::string localisedName;

        if (object->hasAttribute("name" + lang))
            localisedName = object->getStringValue("name" + lang);
        else
            localisedName = object->getStringValue("name");

        std::string name = object->getStringValue("name");
        std::string texture = object->getStringValue("texture");
        world::BuildingType type = magic_enum::enum_cast<world::BuildingType>(object->getStringValue("building")).value();
        world::ProductType productType = magic_enum::enum_cast<world::ProductType>(object->getStringValue("type")).value();

        ProductionCycle cycle = convertJsonObject2Cycle(object->getObjectValue("productionCycle"));
        auto product = std::make_shared<Product>(localisedName, name, texture, type, cycle, productType);
        auto attrs = object->getAttributes();
        if (std::find(attrs.begin(), attrs.end(), std::string("baseResources")) != attrs.end())
        {
            auto baseResources = object->getObjectValue("baseResources");
            for (auto attr : baseResources->getAttributes())
            {
                product->addResource(ResourceService::Instance().getResourceByName(attr), baseResources->getIntValue(attr));
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

    std::shared_ptr<Product> ProductService::getProductByName(const std::string &name)
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
