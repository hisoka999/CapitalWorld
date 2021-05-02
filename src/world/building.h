#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <engine/graphics/rect.h>
#include "../world/product.h"
#include "../world/storage.h"
#include "buildingtypes.h"

class GameMap;

namespace world
{

    struct ProductBalance
    {
        int month;
        int year;
        std::string name;
        double costs;
        double income;
    };

    class Building
    {
    public:
        Building(std::string name, std::string description, int buildPirce, BuildingType type, int blockWidth, int blockHeight);
        Building(std::string name, std::string description, int buildPirce, BuildingType type);
        Building(const Building &copy);

        std::string getName();
        std::string getDescription();
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
        void updateProduction(int month, int year);
        bool isAutoSellActive();
        void autoSell(int month, int year);
        BuildingType getType();
        Storage &getStorage();
        const std::string &getSubTexture();
        void setSubTexture(const std::string &tex);

        virtual void update(GameMap *gameMap){};

    private:
        std::string name;
        BuildingType type;
        std::string description;
        int buildPrice;
        graphics::Rect sourceRect;
        graphics::Rect displayRect;
        int blockWidth, blockHeight;
        int xOffset, yOffset;
        std::vector<std::shared_ptr<Product>> products;
        std::vector<ProductBalance> balance;
        Storage storage;
        std::string subTexture;
    };
}
#endif // BUILDING_H
