#pragma once

#include <engine/ui/Tab.h>
#include "world/building.h"
#include "world/buildings/transportoffice.h"
#include "ui/routecomponent.h"
#include <engine/ui/scrollarea.h>

class GameMap;

namespace UI
{

    class RoutesTab : public UI::Tab
    {
    public:
        RoutesTab(UI::Object *parent, const std::shared_ptr<world::Building> &building, GameMap *gameMap);

        void setBuilding(const std::shared_ptr<world::Building> &building);

    private:
        void initUI();
        void refreshComponents();
        std::shared_ptr<world::buildings::TransportOffice> building;
        std::shared_ptr<UI::ScrollArea> scrollArea;
        GameMap *gameMap;
    };
} // namespace name
