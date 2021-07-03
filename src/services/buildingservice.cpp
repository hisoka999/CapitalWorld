#include "buildingservice.h"
#include "world/buildings/transportoffice.h"
#include "magic_enum.hpp"
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
                index++;
                std::shared_ptr<world::Building> clone = std::make_shared<world::Building>(*building);
                clone->setDisplayName(building->getDisplayName() + " " + std::to_string(index));
                return clone;
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

        switch (buildingClass)
        {
        case world::BuildingClass::Building:
            building = std::make_shared<world::Building>(name, displayName, description, buildCosts, type, width, height);

            break;
        case world::BuildingClass::TransportOffice:
            building = std::make_shared<world::buildings::TransportOffice>(name, displayName, description, buildCosts, type, width, height);
        case world::BuildingClass::Street:
        default:
            break;
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

} // namespace services
