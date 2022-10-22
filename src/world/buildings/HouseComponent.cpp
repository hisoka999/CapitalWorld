#include "HouseComponent.h"
#include "world/building.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#include <regex>
#include <world/gamestate.h>
#include "SalesComponent.h"
#include "StorageComponent.h"
#include "services/productservice.h"
namespace world
{
    namespace buildings
    {
        HouseComponent::HouseComponent(/* args */)
            : BuildingComponent("HouseComponent"), residents(0), houseId(1)
        {
            baseDemand[world::ProductType::Food] = 0.7f;
            baseDemand[world::ProductType::Resource] = 0.3f;

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

            // step one find shops
            if (isGameRunning())
            {
                auto shops = getGameState()->getGameMap()->findByComponentTypeInDistance("SalesComponent", building, 10);

                for (auto &shop : shops)
                {
                    const auto &storage = shop->getComponent<buildings::StorageComponent>("StorageComponent");
                    const auto &sales = shop->getComponent<buildings::SalesComponent>("SalesComponent");

                    for (auto &[storedProduct, entry] : storage->getEntries())
                    {
                        if (!sales->isSalesActive(storedProduct))
                            continue;

                        int storedAmount = entry.amount;
                        if (storedAmount > 0)
                        {
                            auto product = services::ProductService::Instance().getProductByName(storedProduct);
                            int demand = getCurrentDemand(product->getProductType());
                            if (demand < storedAmount)
                                storedAmount = demand;
                            fullfillDemand(product->getProductType(), storedAmount);

                            storage->addEntry(storedProduct, storedAmount * -1);
                            float orderAmount = storedAmount * sales->getSalesPrice(storedProduct);
                            shop->getBalance().addIncome(month, year, storedProduct, world::BalanceAccount::Sales, orderAmount);
                        }
                    }
                }
            }
            // check demands fullfillded
            bool fullfilled = true;
            unsigned int oldResidents = residents;
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
            if (oldResidents != residents)
            {
                updateTexture(building);
                // update demand
                for (auto base : baseDemand)
                {
                    demand[base.first].maxDemand = base.second * residents;
                }
            }
        }

        void HouseComponent::updateTexture(Building *building)
        {

            std::string size = "";

            unsigned int factor = 1;
            auto &buildingPosition = building->get2DPosition();
            if (buildingPosition.width > 1)
            {
                size = "2x2_";
                factor = 4;
            }
            if (residents >= 2000 * factor)
            {
                building->setSubTexture("bighouse_" + size + std::to_string(houseId));
            }
            else if (residents >= 1000 * factor)
            {
                building->setSubTexture("medhouse_" + size + std::to_string(houseId));
            }
            else
            {
                building->setSubTexture("house_" + size + std::to_string(houseId));
            }
            auto textureMap = graphics::TextureManager::Instance().loadTextureMap(utils::os::combine("images", "tiles", "iso_tiles.json"));
            graphics::Rect srcRect;
            textureMap->getSourceRect(building->getSubTextureHash(), &srcRect);
            auto &offset = textureMap->getOffset(building->getSubTextureHash());
            building->setSourceRect(srcRect);
            building->setOffset(offset.getX(), offset.getY());
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