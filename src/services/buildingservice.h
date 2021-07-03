#ifndef __BUILDINGSERVICE_H__
#define __BUILDINGSERVICE_H__

#include "../world/building.h"
#include <memory>
#include <mutex>
#include <engine/utils/json/jsonservice.h>
namespace services
{
    class BuildingService : public utils::json::JSONService<world::Building>
    {
    public:
        static BuildingService &Instance()
        {
            static BuildingService instance;
            return instance;
        }

        std::shared_ptr<world::Building> create(world::BuildingType type);
        void init();

    protected:
        virtual std::shared_ptr<world::Building> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object);

    private:
        BuildingService() = default;
        ~BuildingService() = default;
        BuildingService(const BuildingService &) = delete;
        BuildingService &operator=(const BuildingService &) = delete;
        size_t index = 0;
    };

} // namespace services

#endif // __BUILDINGSERVICE_H__