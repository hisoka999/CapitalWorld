#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <engine/graphics/rect.h>

namespace world {


class Building
{
public:
    Building(std::string name,std::string description, int buildPirce);

    std::string getName();
    std::string getDescription();
    int getBuildPrice();

    bool canBuild(float money);

    graphics::Rect getDisplayRect();
    graphics::Rect getSourceRect();
    void setSourceRect(graphics::Rect rect);
    void setPosition(float x,float y);
    graphics::Rect get2DPosition(int tileWidth,int tileHeight);

private:
    std::string name;
    std::string description;
    int buildPrice;
    graphics::Rect sourceRect;
    graphics::Rect displayRect;

};
}
#endif // BUILDING_H
