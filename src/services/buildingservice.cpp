#include "buildingservice.h"
#include "magic_enum.hpp"
#include "world/buildings/TransportComponent.h"
#include <algorithm>
#include <engine/utils/localisation.h>

namespace services
{
    std::shared_ptr<world::Building> BuildingService::create(world::BuildingType type)
    {

        for (auto &building : getData())
        {
            if (building->getType() == type)
            {
                return create(building);
            }
        }
        return nullptr;
    }

    std::shared_ptr<world::Building> BuildingService::create(std::shared_ptr<world::Building> original)
    {
        index++;
        std::shared_ptr<world::Building> clone;
        clone = std::make_shared<world::Building>(*original);

        clone->setDisplayName(original->getDisplayName() + " " + std::to_string(index));
        return clone;
    }

    std::vector<std::shared_ptr<world::Building>> BuildingService::find(world::BuildingType type)
    {
        std::vector<std::shared_ptr<world::Building>> result;
        for (auto &building : getData())
        {
            if (building->getType() == type)
            {
                result.push_back(building);
            }
        }
        return result;
    }

    std::shared_ptr<world::Building> BuildingService::findByName(const std::string &name)
    {
        for (auto &building : getData())
        {
            if (building->getName() == name)
            {
                return building;
            }
        }
        return nullptr;
    }

    void BuildingService::init()
    {
        index = 0;
    }

    std::shared_ptr<world::Building> BuildingService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
    {
        std::string lang = Localisation::Instance().getLanguage();
        if (lang == "en")
            lang = "";
        else
        {
            lang = "_" + lang;
        }
        std::string name = object->getStringValue("name");
        std::string displayName = object->getStringValue("displayName" + lang);
        std::string description = object->getStringValue("description" + lang);
        int buildCosts = object->getIntValue("buildCosts");
        std::shared_ptr<world::Building> building;
        object->getStringValue("buildingClass");
        world::BuildingClass buildingClass = magic_enum::enum_cast<world::BuildingClass>(object->getStringValue("buildingClass")).value();
        world::BuildingType type = magic_enum::enum_cast<world::BuildingType>(object->getStringValue("type")).value();
        int width = object->getIntValue("block_width");
        int height = object->getIntValue("block_height");

        building = std::make_shared<world::Building>(name, displayName, description, buildCosts, type, width, height);
        auto components = object->getObjectValue("components");
        for (auto &componentName : components->getAttributes())
        {
            auto metaData = components->getObjectValue(componentName);
            // auto componentName = std::get<std::string>(componentValue);
            auto component = world::Building::createComponentByName(componentName);
            component->setMetaData(metaData);
            building->addComponent(component);
        }

        if (object->hasArray("rawResources"))
        {
            for (auto resource : object->getArray("rawResources"))
            {
                world::RawResource rawResource = magic_enum::enum_cast<world::RawResource>(std::get<std::string>(resource)).value();
                building->addResource(rawResource);
            }
        }

        graphics::Rect rect;
        auto sourceRect = object->getObjectValue("sourceRect");
        rect.x = sourceRect->getIntValue("x");
        rect.y = sourceRect->getIntValue("y");
        rect.width = sourceRect->getIntValue("width");
        rect.height = sourceRect->getIntValue("height");
        auto offset = object->getObjectValue("offset");
        building->setOffset(offset->getIntValue("x"), offset->getIntValue("y"));
        building->setSourceRect(rect);

        return building;
    }

    BuildingService::BuildingService()
    {
    }

} // namespace services
