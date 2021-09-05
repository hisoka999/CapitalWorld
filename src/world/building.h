#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <engine/graphics/rect.h>
#include "world/product.h"
#include "buildingtypes.h"
#include <engine/utils/json/object.h>
#include "world/buildings/BuildingComponent.h"
class GameMap;

namespace world
{
    class Company;

    enum class BalanceAccount
    {
        Production,
        Transport,
        Import,
        Sales,
    };

    struct ProductBalance
    {
        int month;
        int year;
        std::string name;
        double costs;
        double income;
        BalanceAccount account;
    };

    enum class BuildingClass
    {
        Building,
        TransportOffice,
        Street,
        Farm
    };

    class Building
    {
    public:
        Building(std::string name, std::string displayName, std::string description, int buildPrice, BuildingType type, int blockWidth, int blockHeight);
        Building(std::string name, std::string displayName, std::string description, int buildPrice, BuildingType type);
        Building(const Building &copy);

        std::string getName();
        std::string getDescription();
        const std::string &getDisplayName() const;
        void setDisplayName(const std::string name);
        int getBuildPrice();

        bool canBuild(float money);

        graphics::Rect &getDisplayRect();
        graphics::Rect &getSourceRect();
        void setSourceRect(graphics::Rect rect);
        void setPosition(float x, float y);
        graphics::Rect get2DPosition();
        int getXOffset();
        int getYOffset();
        void setOffset(int x, int y);
        void addProduct(std::shared_ptr<Product> product);
        void removeProduct(std::shared_ptr<Product> product);
        bool hasProduct(std::shared_ptr<Product> product);
        std::vector<std::shared_ptr<Product>> getProducts();
        void calculateBalance(int month, int year);
        float getCostsPerMonth(int month, int year);
        float getIncomePerMonth(int month, int year);
        virtual void updateProduction(int month, int year);
        void addCosts(int month, int year, const std::string &productName, BalanceAccount account, int amount);
        void addIncome(int month, int year, const std::string &productName, BalanceAccount account, int amount);
        bool isAutoSellActive();
        void autoSell(int month, int year);
        BuildingType getType();
        //Storage &getStorage();
        const std::string &getSubTexture();
        const size_t getSubTextureHash();
        void setSubTexture(const std::string &tex);

        virtual void update(GameMap *gameMap){};

        virtual std::shared_ptr<utils::JSON::Object> toJson();
        static std::shared_ptr<Building> fromJson(const std::shared_ptr<Building> &reference, const std::shared_ptr<utils::JSON::Object> &object, world::Company *company);
        void addComponent(std::shared_ptr<world::buildings::BuildingComponent> &component);
        std::shared_ptr<world::buildings::BuildingComponent> getComponentByName(const std::string &name);
        bool hasComponent(const std::string &name);

        template <typename T>
        std::shared_ptr<T> getComponent(const std::string &name)
        {
            return std::dynamic_pointer_cast<T>(getComponentByName(name));
        }
        static void initComponentMap();
        static std::shared_ptr<world::buildings::BuildingComponent> createComponentByName(const std::string &name);

        void delayedUpdate(Company *company);

    protected:
        void addBalance(ProductBalance value);

    private:
        std::string name;
        std::string displayName;
        BuildingType type;
        std::string description;
        int buildPrice;
        graphics::Rect sourceRect;
        graphics::Rect displayRect;
        int blockWidth, blockHeight;
        int xOffset, yOffset;
        std::vector<std::shared_ptr<Product>> products;
        std::vector<ProductBalance> balance;
        std::string subTexture;
        std::map<std::string, std::shared_ptr<world::buildings::BuildingComponent>> components;
        static std::map<std::string, std::shared_ptr<world::buildings::BuildingComponent>> componentMap;
        std::hash<std::string> hasher;
        size_t hashId = 0;
    };
}
#endif // BUILDING_H
