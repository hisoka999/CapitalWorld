#ifndef BUILDINGWINDOW_H
#define BUILDINGWINDOW_H

#include <engine/ui/Window.h>
#include <engine/ui/TabBar.h>
#include <world/building.h>
#include <world/company.h>
#include <engine/ui/Label.h>
#include <world/gamemap.h>

namespace UI {

class BuildingWindow : public UI::Window
{
public:
    BuildingWindow(int x,int y);

    void open(std::shared_ptr<world::Building> building,std::shared_ptr<world::Company> company,TileType tile);
private:
    UI::TabBar tabBar;
    TileType tile;
    std::shared_ptr<world::Building> building;
    UI::Label* labelTypeValue;
    UI::Label* labelOwnerValue;
    UI::Label* labelGroundValue;
    std::shared_ptr<UI::Tab> productionTab;
};

}
#endif // BUILDINGWINDOW_H
