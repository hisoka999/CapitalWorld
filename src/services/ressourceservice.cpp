#include "ressourceservice.h"
#include <magic_enum.hpp>
namespace services
{
    std::shared_ptr<Resource> RessourceService::getResourceByName(std::string name)
    {
        for (auto &data : getData())
        {
            if (data->getName() == name)
                return data;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<Resource>> RessourceService::getResourcesByBuildingType(world::BuildingType type)
    {
        std::vector<std::shared_ptr<Resource>> result;
        for (auto &res : getData())
        {
            if (res->getBuildingType() == type)
            {
                result.push_back(res);
            }
        }

        return result;
    }

    RessourceService::RessourceService()
    {
    }

    std::shared_ptr<Resource> RessourceService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
    {
        std::string name = object->getStringValue("name");
        std::string texture = object->getStringValue("texture");
        int costs = object->getIntValue("costs");
        world::BuildingType type = magic_enum::enum_cast<world::BuildingType>(object->getStringValue("building")).value();
        return std::make_shared<Resource>(name, texture, costs, type);

        //Resource(std::string name, std::string image, float costPerMonth, world::BuildingType buildingType);
    }
}