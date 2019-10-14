#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <engine/graphics/rect.h>
#include <world/product.h>

namespace world {

struct ProductBalance{
    int month;
    int year;
    std::string name;
    float costs;
    float income;
};

class Building
{
public:
    Building(std::string name,std::string description, int buildPirce,int blockWidth,int blockHeight);
    Building(std::string name,std::string description, int buildPirce);

    std::string getName();
    std::string getDescription();
    int getBuildPrice();

    bool canBuild(float money);

    graphics::Rect getDisplayRect();
    graphics::Rect getSourceRect();
    void setSourceRect(graphics::Rect rect);
    void setPosition(float x,float y);
    graphics::Rect get2DPosition();
    int getXOffset();
    int getYOffset();
    void setOffset(int x,int y);
    void addProduct(std::shared_ptr<Product> product);
    void removeProduct(std::shared_ptr<Product> product);
    bool hasProduct(std::shared_ptr<Product> product);
    std::vector<std::shared_ptr<Product>> getProducts();
    void calculateBalance(int month,int year);
    float getCostsPerMonth(int month,int year);

private:
    std::string name;
    std::string description;
    int buildPrice;
    graphics::Rect sourceRect;
    graphics::Rect displayRect;
    int blockWidth,blockHeight;
    int xOffset,yOffset;
    std::vector<std::shared_ptr<Product>> products;
    std::vector<ProductBalance> balance;
};
}
#endif // BUILDING_H
