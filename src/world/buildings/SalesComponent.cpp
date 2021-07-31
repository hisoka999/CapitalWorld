#include "SalesComponent.h"
#include "world/building.h"
#include <algorithm>
#include "StorageComponent.h"
#include "world/gamemap.h"
#include "HouseComponent.h"

namespace world
{
    namespace buildings
    {
        int SalesComponent::calcDemand(const std::string &product, Building *building)
        {

            if (gameMap != nullptr)
            {
                int residents = 0;
                auto houses = gameMap->findHousesInDistance(building, 10);
                for (auto house : houses)
                {
                    auto comp = house->getComponent<world::buildings::HouseComponent>("HouseComponent");
                    residents += comp->getResidents();
                }

                return residents * 0.2;
            }
        }

        bool SalesComponent::isSalesActive(const std::string &product)
        {
            for (auto &item : sales)
            {
                if (item->product == product)
                {
                    return item->active;
                }
            }
        }

        SalesComponent::SalesComponent(/* args */)
            : BuildingComponent("SalesComponent"), gameMap(nullptr)
        {
        }

        SalesComponent::~SalesComponent()
        {
        }

        void SalesComponent::setGameMap(GameMap *gameMap)
        {
            this->gameMap = gameMap;
        }

        void SalesComponent::addSalesItem(const std::string &product, const float price)
        {
            sales.push_back(std::make_shared<SalesItem>(product, price));
        }

        void SalesComponent::updateSalesItem(const std::string &product, const float price)
        {
            for (auto &item : sales)
            {
                if (item->product == product)
                {
                    item->price = price;
                    break;
                }
            }
        }

        void SalesComponent::removeSalesItem(const std::string &product)
        {
            auto it = std::find_if(sales.begin(), sales.end(), [&](std::shared_ptr<SalesItem> &v)
                                   { return v->product == product; });
            if (it != std::end(sales))
            {
                sales.erase(it);
            }
        }

        void SalesComponent::removeSalesItem(const size_t position)
        {
            auto it = sales.begin() + position;
            if (it != std::end(sales))
            {
                sales.erase(it);
            }
        }

        float SalesComponent::getSalesPrice(const std::string &productName)
        {
            for (auto &item : sales)
            {
                if (item->product == productName)
                {
                    return item->price;
                }
            }
            return 0;
        }

        void SalesComponent::updateProduction(int month, int year, Building *building)
        {
            // how to determen builings in the near area >> houses

            auto storage = building->getComponent<world::buildings::StorageComponent>("StorageComponent");
            for (auto &storedProduct : storage->getStoredProducts())
            {
                if (!isSalesActive(storedProduct))
                    continue;
                int amount = storage->getEntry(storedProduct);
                int demand = calcDemand(storedProduct, building);
                if (amount > 0)
                {
                    if (amount > demand)
                    {
                        amount = demand;
                    }
                    float income = amount * getSalesPrice(storedProduct);
                    if (income > 0)
                    {
                        storage->addEntry(storedProduct, amount * -1);
                        building->addIncome(month, year, storedProduct, BalanceAccount::Sales, income);
                    }
                }
            }
        }

        std::shared_ptr<BuildingComponent> SalesComponent::clone()
        {
            return std::make_shared<SalesComponent>();
        }

        std::vector<std::shared_ptr<SalesItem>> &SalesComponent::getSalesItems()
        {
            return sales;
        }

        std::shared_ptr<utils::JSON::Object> SalesComponent::toJson()
        {
            auto object = world::buildings::BuildingComponent::toJson();

            for (auto &item : sales)
            {
                object->setAttribute(item->product, item->price);
            }
            return object;
        }

        void SalesComponent::fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company)
        {
            for (auto attr : object->getAttributes())
            {
                if (attr != "name")
                    addSalesItem(attr, object->getFloatValue(attr));
            }
        }

    }
}