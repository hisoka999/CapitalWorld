#ifndef BUILDING_H
#define BUILDING_H

#include "buildingtypes.h"
#include "world/buildings/BuildingComponent.h"
#include "world/product.h"
#include <engine/graphics/rect.h>
#include <engine/utils/json/object.h>
#include <string>
#include "balance.h"

class GameMap;

namespace world
{
    class Company;

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

        const graphics::Rect &getDisplayRect();
        graphics::Rect &getSourceRect();
        void setSourceRect(graphics::Rect rect);
        void setPosition(float x, float y);
        graphics::Rect &get2DPosition();
        int getXOffset();
        int getYOffset();
        void setOffset(int x, int y);
        void addProduct(const std::shared_ptr<Product> &product);
        void removeProduct(const std::shared_ptr<Product> &product);
        bool hasProduct(const std::shared_ptr<Product> &product);
        std::vector<std::shared_ptr<Product>> getProducts();

        virtual void updateProduction(unsigned int month, unsigned int year);

        bool isAutoSellActive();
        void autoSell(int month, int year);
        BuildingType getType();
        // Storage &getStorage();
        const std::string &getSubTexture();
        size_t getSubTextureHash();
        void setSubTexture(const std::string &tex);

        virtual void update([[maybe_unused]] GameMap *gameMap){};

        virtual std::shared_ptr<utils::JSON::Object> toJson();
        static std::shared_ptr<Building> fromJson(const std::shared_ptr<Building> &reference, const std::shared_ptr<utils::JSON::Object> &object, world::Company *company);
        void addComponent(std::shared_ptr<world::buildings::BuildingComponent> &component);
        const std::shared_ptr<world::buildings::BuildingComponent> &getComponentByName(const std::string &name);
        bool hasComponent(const std::string &name);

        template <typename T>
        std::shared_ptr<T> getComponent(const std::string &name)
        {
            return std::static_pointer_cast<T>(getComponentByName(name));
        }
        static void initComponentMap();
        static std::shared_ptr<world::buildings::BuildingComponent> createComponentByName(const std::string &name);

        void delayedUpdate(Company *company);

        std::map<std::string, std::string> displayData();
        bool requireResource(world::RawResource rawResource);
        void addResource(world::RawResource rawResource);
        Balance &getBalance();
        bool canDragBuild();
        utils::Vector2 getPosition();

    protected:
        bool m_dragBuild = false;

    private:
        std::string name;
        std::string displayName;
        BuildingType type;
        std::string description;
        int buildPrice;
        graphics::Rect sourceRect;
        graphics::Rect displayRect;
        graphics::Rect twoDRect;
        int blockWidth, blockHeight;
        int xOffset, yOffset;
        std::vector<std::shared_ptr<Product>> products;
        std::string subTexture;
        std::map<std::string, std::shared_ptr<world::buildings::BuildingComponent>> components;
        static std::map<std::string, std::shared_ptr<world::buildings::BuildingComponent>> componentMap;
        std::hash<std::string> hasher;
        size_t hashId = 0;
        std::vector<world::RawResource> rawResources;
        Balance m_balance;
    };
}
#endif // BUILDING_H
