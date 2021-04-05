#ifndef BUILDINGWINDOW_H
#define BUILDINGWINDOW_H

#include <engine/ui/Window.h>
#include <engine/ui/TabBar.h>
#include "../world/building.h"
#include "../world/company.h"
#include <engine/ui/Label.h>
#include "../world/gamemap.h"

namespace UI
{

    class BuildingWindow : public UI::Window
    {
    public:
        BuildingWindow(int x, int y);

        void open(std::shared_ptr<world::Building> building, std::shared_ptr<world::Company> company, TileType tile);

    private:
        std::shared_ptr<UI::TabBar> tabBar;
        TileType tile;
        std::shared_ptr<world::Building> building;
        std::shared_ptr<UI::Label> labelTypeValue;
        std::shared_ptr<UI::Label> labelOwnerValue;
        std::shared_ptr<UI::Label> labelGroundValue;
        std::shared_ptr<UI::Tab> productionTab;
        std::shared_ptr<UI::Tab> storageTab;
    };

}
#endif // BUILDINGWINDOW_H
