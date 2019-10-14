#include "building.h"
#include <algorithm>

namespace world {

Building::Building(std::string name,std::string description, int buildPirce,int blockWidth,int blockHeight)
    :name(name),description(description),buildPrice(buildPirce),blockWidth(blockWidth),blockHeight(blockHeight),xOffset(0),yOffset(0)
{

}


Building::Building(std::string name,std::string description, int buildPirce)
    :name(name),description(description),buildPrice(buildPirce),blockWidth(1),blockHeight(1),xOffset(0),yOffset(0)
{

}


std::string Building::getName()
{
    return name;
}
std::string Building::getDescription()
{
    return description;
}
int Building::getBuildPrice()
{
    return buildPrice;
}

bool Building::canBuild(float money)
{
    return buildPrice <= money;
}

graphics::Rect Building::getDisplayRect()
{
    return displayRect;
}
graphics::Rect Building::getSourceRect()
{
    return sourceRect;
}
void Building::setSourceRect(graphics::Rect rect)
{
    sourceRect = rect;
}
void Building::setPosition(float x,float y)
{
    displayRect.x = x;
    displayRect.y = y;
    displayRect.width = sourceRect.width;
    displayRect.height = sourceRect.height;
}

graphics::Rect Building::get2DPosition()
{
    graphics::Rect r = displayRect;
    r.width = blockWidth;
    r.height = blockHeight;
    r.x-= blockWidth;
    r.y-=blockHeight;

    return r;
}
int Building::getXOffset()
{
    return xOffset;
}
int Building::getYOffset()
{
    return yOffset;
}
void Building::setOffset(int x,int y)
{
    xOffset = x;
    yOffset = y;
}
void Building::addProduct(std::shared_ptr<Product> product)
{
    if(!hasProduct(product))
        products.push_back(product);
}
void Building::removeProduct(std::shared_ptr<Product> product)
{
    auto it = std::find(products.begin(),products.end(),product);
    if(it != products.end())
    {
        products.erase(it);
    }
}
std::vector<std::shared_ptr<Product>> Building::getProducts()
{
    return products;
}

bool Building::hasProduct(std::shared_ptr<Product> product)
{
    auto it = std::find(products.begin(),products.end(),product);
    return it != products.end();
}
void Building::calculateBalance(int month,int year)
{
    for(auto& product : products)
    {
        ProductBalance producBalance;
        producBalance.name = product->getName();
        producBalance.year = year;
        producBalance.month = month;
        producBalance.costs = product->calculateCostsPerMonth();
        producBalance.income = 0;//TODO
        balance.push_back(producBalance);
    }
}
float Building::getCostsPerMonth(int month,int year)
{
    float result = .0f;
    for (auto& b : balance)
    {
        if(b.month == month
                && b.year == year){
            result+=b.costs;
        }
    }
    return result;
}

}
