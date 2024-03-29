#include "SalesComponent.h"
#include "HouseComponent.h"
#include "StorageComponent.h"
#include "services/productservice.h"
#include "world/building.h"
#include "world/gamemap.h"
#include <algorithm>
namespace world
{
    namespace buildings
    {
        int SalesComponent::calcDemand([[maybe_unused]] const ProductType productType, [[maybe_unused]] Building *building)
        {

            // if (gameMap != nullptr)
            // {
            //     int demand = 0;
            //     auto houses = gameMap->findHousesInDistance(building, 10);
            //     for (auto house : houses)
            //     {
            //         auto comp = house->getComponent<world::buildings::HouseComponent>("HouseComponent");
            //         demand += comp->getCurrentDemand(productType);
            //     }

            //     return demand;
            // }
            return -1;
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
            return false;
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
            addSalesItem(product, price, false);
        }

        void SalesComponent::addSalesItem(const std::string &product, const float price, const bool active)
        {
            if (sales.size() == MAX_SALES_ITEMS)
                return;

            sales.push_back(std::make_shared<SalesItem>(product, price, active));
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

        void SalesComponent::updateProduction([[maybe_unused]] int month, [[maybe_unused]] int year, [[maybe_unused]] Building *building)
        {
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

            utils::JSON::JsonArray items;
            for (auto &item : sales)
            {
                std::shared_ptr<utils::JSON::Object> salesItem = std::make_shared<utils::JSON::Object>();
                salesItem->setAttribute("product", item->product);
                salesItem->setAttribute("price", item->price);
                salesItem->setAttribute("active", item->active);
                items.push_back(salesItem);
            }
            object->setArrayAttribute("items", items);
            return object;
        }

        void SalesComponent::fromJson(std::shared_ptr<utils::JSON::Object> &object, [[maybe_unused]] Company *company)
        {
            auto items = object->getArray("items");
            for (auto item : items)
            {
                auto value = std::get<std::shared_ptr<utils::JSON::Object>>(item);
                std::string product = value->getStringValue("product");
                float price = value->getFloatValue("price");
                bool active = value->getBoolValue("active");
                addSalesItem(product, price, active);
            }
        }
    }
}
