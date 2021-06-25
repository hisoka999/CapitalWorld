
#pragma once
#include <engine/ui/Container.h>
#include <engine/ui/Label.h>
#include <engine/ui/iconbutton.h>
#include <engine/ui/ComboBox.h>
#include "world/buildings/transportoffice.h"

class GameMap;

namespace UI
{
#pragma once
    class RouteComponent : public UI::Container, public UI::Object
    {
    public:
        RouteComponent(UI::Object *parent, const std::shared_ptr<world::buildings::TransportRoute> &route, GameMap *gameMap, const std::shared_ptr<world::Building> &parentBuilding);
        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);

    private:
        void fillProductListByBuilding(std::shared_ptr<world::Building> &building);
        void initUI();
        std::shared_ptr<world::buildings::TransportRoute> route;
        GameMap *gameMap;
        std::shared_ptr<world::Building> parentBuilding;
        std::shared_ptr<UI::ComboBox<std::shared_ptr<Product>>> productList;
    };
};