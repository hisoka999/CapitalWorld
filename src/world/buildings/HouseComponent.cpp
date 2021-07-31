#include "HouseComponent.h"
#include "world/building.h"
namespace world
{
    namespace buildings
    {
        HouseComponent::HouseComponent(/* args */)
            : BuildingComponent("HouseComponent"), residents(0)
        {
        }

        HouseComponent::HouseComponent(unsigned int residents)
            : BuildingComponent("HouseComponent"), residents(residents)
        {
        }

        HouseComponent::~HouseComponent()
        {
        }

        unsigned int HouseComponent::getResidents()
        {
            return residents;
        }

        void HouseComponent::fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company)
        {
            residents = object->getIntValue("residents");
        }

        void HouseComponent::updateProduction(int month, int year, Building *building)
        {
        }

        std::shared_ptr<BuildingComponent> HouseComponent::clone()
        {
            return std::make_shared<HouseComponent>();
        }

        std::shared_ptr<utils::JSON::Object> HouseComponent::toJson()
        {
            auto object = world::buildings::BuildingComponent::toJson();
            object->setAttribute("residents", int(residents));
            return object;
        }
    }
}