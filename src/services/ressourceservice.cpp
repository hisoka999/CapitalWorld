#include "ressourceservice.h"
#include <magic_enum.hpp>
#include <engine/utils/localisation.h>

namespace services
{
    std::shared_ptr<Resource> ResourceService::getResourceByName(std::string name)
    {
        for (auto &data : getData())
        {
            if (data->getName() == name)
                return data;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<Resource>> ResourceService::getResourcesByBuildingType(world::BuildingType type, world::RawResource rawResource)
    {
        std::vector<std::shared_ptr<Resource>> result;
        for (auto &res : getData())
        {
            if (res->getBuildingType() == type && res->getRawResource() == rawResource)
            {
                result.push_back(res);
            }
        }

        return result;
    }

    ResourceService::ResourceService()
    {
    }

    std::shared_ptr<Resource> ResourceService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
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
        int costs = object->getIntValue("costs");
        world::BuildingType type = magic_enum::enum_cast<world::BuildingType>(object->getStringValue("building")).value();

        world::RawResource raw = world::RawResource::None;
        if (object->hasAttribute("rawResource"))
        {
            raw = magic_enum::enum_cast<world::RawResource>(object->getStringValue("rawResource")).value();
        }

        return std::make_shared<Resource>(localisedName, name, texture, costs, type, raw);

        // Resource(std::string name, std::string image, float costPerMonth, world::BuildingType buildingType);
    }
}