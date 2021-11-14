#pragma once

#include <engine/utils/json/jsonservice.h>
#include <world/ressource.h>
namespace services
{
    class ResourceService : public utils::json::JSONService<Resource>
    {
    private:
        /* data */
    public:
        static ResourceService &Instance()
        {
            static ResourceService instance;
            return instance;
        }
        std::shared_ptr<Resource> getResourceByName(std::string name);
        std::vector<std::shared_ptr<Resource>> getResourcesByBuildingType(world::BuildingType type, world::RawResource rawResource);

    private:
        ResourceService();
        ~ResourceService() = default;
        ResourceService(const ResourceService &) = delete;
        ResourceService &operator=(const ResourceService &) = delete;

    protected:
        virtual std::shared_ptr<Resource> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object);
    };

};