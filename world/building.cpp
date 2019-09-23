#include "building.h"

namespace world {

Building::Building(std::string name,std::string description, int buildPirce)
    :name(name),description(description),buildPrice(buildPirce)
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

graphics::Rect Building::get2DPosition(int tileWidth,int tileHeight)
{
    graphics::Rect r = displayRect;
    r.width /= tileWidth;
    r.height /= tileHeight;

    return r;
}


}
