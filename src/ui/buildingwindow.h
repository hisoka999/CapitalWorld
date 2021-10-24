#ifndef BUILDINGWINDOW_H
#define BUILDINGWINDOW_H

#include "../world/building.h"
#include "../world/company.h"
#include "../world/gamemap.h"
#include <engine/ui/Label.h>
#include <engine/ui/TabBar.h>
#include <engine/ui/Window.h>
#include <engine/ui/layout/Layout.h>

class GameMap;

namespace UI
{

    class BuildingWindow : public UI::Window
    {
    public:
        BuildingWindow(int x, int y);

        void open(std::shared_ptr<world::Building> building, std::shared_ptr<world::Company> company, TileType tile, GameMap *gameMap);

    private:
        std::shared_ptr<UI::TabBar> tabBar;
        TileType tile;
        std::shared_ptr<world::Building> building;
        std::shared_ptr<UI::Label> labelTypeValue;
        std::shared_ptr<UI::Label> labelOwnerValue;
        std::shared_ptr<UI::Label> labelGroundValue;
        std::shared_ptr<UI::Tab> productionTab;
        std::shared_ptr<UI::Tab> storageTab;
        std::shared_ptr<UI::Tab> infoTab;
        std::shared_ptr<UI::Tab> workerTab;
        std::shared_ptr<UI::layout::Layout> layout;
        std::vector<std::shared_ptr<UI::Object>> optionalItems;
    };

}
#endif // BUILDINGWINDOW_H
