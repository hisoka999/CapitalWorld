#include "HouseComponent.h"
#include "world/building.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#include <regex>
namespace world
{
    namespace buildings
    {
        HouseComponent::HouseComponent(/* args */)
            : BuildingComponent("HouseComponent"), residents(0), houseId(1)
        {
            baseDemand[world::ProductType::Food] = 0.7f;

            initDemand();
        }

        HouseComponent::HouseComponent(unsigned int residents, DemandMap baseDemand, int houseId)
            : BuildingComponent("HouseComponent"), residents(residents), baseDemand(baseDemand), houseId(houseId)
        {
            initDemand();
        }

        HouseComponent::~HouseComponent()
        {
        }

        unsigned int HouseComponent::getResidents()
        {
            return residents;
        }

        void HouseComponent::fromJson(std::shared_ptr<utils::JSON::Object> &object, [[maybe_unused]] Company *company)
        {
            residents = object->getIntValue("residents");
        }

        void HouseComponent::updateProduction([[maybe_unused]] int month, [[maybe_unused]] int year, [[maybe_unused]] Building *building)
        {
            // check demands fullfillded
            bool fullfilled = true;
            for (auto &d : demand)
            {
                if (d.second.value == d.second.maxDemand)
                {
                    d.second.monthsFullFilled++;
                }
                else
                {
                    fullfilled = false;
                }
                d.second.value = 0;
            }
            if (fullfilled && residents < maxResidents)
            {
                residents *= 1.2;
            }

            updateTexture(building);
            // update demand
            for (auto base : baseDemand)
            {
                demand[base.first].maxDemand = base.second * residents;
            }
        }

        void HouseComponent::updateTexture(Building *building)
        {
            if (residents >= 2000)
            {
                building->setSubTexture("bighouse_" + std::to_string(houseId));
            }
            else if (residents >= 1000)
            {
                building->setSubTexture("medhouse_" + std::to_string(houseId));
            }
            else
            {
                building->setSubTexture("house_" + std::to_string(houseId));
            }
            auto textureMap = graphics::TextureManager::Instance().loadTextureMap(utils::os::combine("images", "tiles", "iso_tiles.json"));
            graphics::Rect srcRect;
            textureMap->getSourceRect(building->getSubTextureHash(), &srcRect);
            building->setSourceRect(srcRect);
            building->setOffset(0, building->getSourceRect().height - 32);
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

        int HouseComponent::getCurrentDemand(world::ProductType type)
        {
            return demand[type].maxDemand - demand[type].value;
        }

        int HouseComponent::fullfillDemand(world::ProductType type, int value)
        {
            if (demand.count(type) == 0)
                return 0;
            demand[type].value += value;
            int offset = demand[type].maxDemand - demand[type].value;
            if (offset < 0)
            {
                demand[type].value = demand[type].maxDemand;
                return 0;
            }
            return offset;
        }

        std::map<std::string, std::string> HouseComponent::displayData()
        {
            std::map<std::string, std::string> result;
            result["residents"] = std::to_string(residents);
            return result;
        }

        void HouseComponent::initDemand()
        {
            for (auto base : baseDemand)
            {
                Demand d;
                d.maxDemand = base.second * residents;
                d.monthsFullFilled = 0;
                d.value = 0;
                demand[base.first] = d;
            }
        }
    }
}