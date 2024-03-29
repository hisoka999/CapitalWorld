#ifndef __SALESCOMPONENT_H__
#define __SALESCOMPONENT_H__

#include "world/buildings/BuildingComponent.h"
#include "world/product.h"

class GameMap;

namespace world
{
    namespace buildings
    {
        const size_t MAX_SALES_ITEMS = 10;
        struct SalesItem
        {
            std::string product;
            float price;
            bool active;

            SalesItem(const std::string &product,
                      const float price) : product(product), price(price), active(false)
            {
            }
            SalesItem(const std::string &product,
                      const float price, bool active) : product(product), price(price), active(active)
            {
            }
        };

        class SalesComponent : public BuildingComponent
        {
        private:
            std::vector<std::shared_ptr<SalesItem>> sales;
            GameMap *gameMap;
            int calcDemand(const ProductType productType, Building *building);

        public:
            SalesComponent(/* args */);
            ~SalesComponent();

            void setGameMap(GameMap *gameMap);
            bool isSalesActive(const std::string &product);
            void addSalesItem(const std::string &product, const float price);
            void addSalesItem(const std::string &product, const float price, const bool active);
            void updateSalesItem(const std::string &product, const float price);
            void removeSalesItem(const std::string &product);
            void removeSalesItem(const size_t position);
            float getSalesPrice(const std::string &productName);
            virtual void updateProduction(int month, int year, Building *building);
            virtual std::shared_ptr<BuildingComponent> clone();
            std::vector<std::shared_ptr<SalesItem>> &getSalesItems();
            virtual std::shared_ptr<utils::JSON::Object> toJson();
            virtual void fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company);
        };

    } // namespace buildings

} // namespace world

#endif // __SALESCOMPONENT_H__