#include "building.h"

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



}
