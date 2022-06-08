
#pragma once
#include <engine/ui/Container.h>
#include <engine/ui/Label.h>
#include <engine/ui/iconbutton.h>
#include <engine/ui/ComboBox.h>
#include "world/buildings/TransportComponent.h"

class GameMap;

namespace UI
{
#pragma once
    class RouteComponent : public UI::Container, public UI::Object
    {
    public:
        RouteComponent(UI::Object *parent, const std::shared_ptr<world::buildings::TransportRoute> &route, GameMap *gameMap, const std::shared_ptr<world::Building> &parentBuilding, const std::shared_ptr<world::Company> &player);
        virtual void render(core::Renderer *pRender);
        virtual bool handleEvents(core::Input *pInput);

        std::shared_ptr<UI::Button> closeButton;

    private:
        void fillProductListByBuilding(std::shared_ptr<world::Building> &building);
        void initUI();
        std::shared_ptr<world::buildings::TransportRoute> route;
        GameMap *gameMap;
        std::shared_ptr<world::Building> parentBuilding;
        std::shared_ptr<UI::ComboBox<std::shared_ptr<Product>>> productList;
        std::shared_ptr<world::Company> player;
    };
};