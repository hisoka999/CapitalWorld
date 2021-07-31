#pragma once

#include <engine/ui/Tab.h>
#include "world/building.h"
#include "ui/routecomponent.h"
#include <engine/ui/scrollarea.h>

class GameMap;

namespace UI
{

    class RoutesTab : public UI::Tab
    {
    public:
        RoutesTab(UI::Object *parent, const std::shared_ptr<world::Building> &building, GameMap *gameMap, const std::shared_ptr<world::Company> &player);

        void setBuilding(const std::shared_ptr<world::Building> &building);

    protected:
        void refresh();

    private:
        void initUI();
        void refreshComponents();
        std::shared_ptr<world::Building> building;
        std::shared_ptr<UI::ScrollArea> scrollArea;
        GameMap *gameMap;
        std::shared_ptr<world::Company> player;
    };
} // namespace name
