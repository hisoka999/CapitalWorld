#pragma once

#include <engine/utils/json/jsonservice.h>
#include <world/ressource.h>
namespace services
{
    class RessourceService : public utils::json::JSONService<Resource>
    {
    private:
        /* data */
    public:
        static RessourceService &Instance()
        {
            static RessourceService instance;
            return instance;
        }
        std::shared_ptr<Resource> getResourceByName(std::string name);
        std::vector<std::shared_ptr<Resource>> getResourcesByBuildingType(world::BuildingType type);

    private:
        RessourceService();
        ~RessourceService() = default;
        RessourceService(const RessourceService &) = delete;
        RessourceService &operator=(const RessourceService &) = delete;

    protected:
        virtual std::shared_ptr<Resource> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object);
    };

};