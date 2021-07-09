#include "routestab.h"
#include <future>

namespace UI
{
    RoutesTab::RoutesTab(UI::Object *parent, const std::shared_ptr<world::Building> &building, GameMap *gameMap) : UI::Tab(parent, "Transport routes"), gameMap(gameMap)
    {
        setWidth(parent->getWidth() - 100);
        setHeight(parent->getHeight() - 10);
        setBuilding(building);
    }

    void RoutesTab::setBuilding(const std::shared_ptr<world::Building> &building)
    {
        this->building = std::dynamic_pointer_cast<world::buildings::TransportOffice>(building);
        initUI();
    }

    void RoutesTab::initUI()
    {
        clear();
        scrollArea = std::make_shared<UI::ScrollArea>(getWidth(), getHeight() - 50, this);
        scrollArea->setPos(0, 0);
        addObject(scrollArea);
        refreshComponents();
        std::shared_ptr<UI::Button> newRouteButton = std::make_shared<UI::Button>(this);
        newRouteButton->setLabel("Add Route");
        newRouteButton->setPos(5, getHeight() - 50);
        newRouteButton->connect(UI::Button::buttonClickCallback(), [&]()
                                {
                                    building->addRoute(nullptr, nullptr, nullptr, 0);
                                    refreshComponents();
                                });
        addObject(newRouteButton);
    }

    void RoutesTab::refreshComponents()
    {
        scrollArea->reset();

        int yoffset = 0;
        if (building == nullptr)
            return;
        for (auto &route : building->getAllRoutes())
        {
            std::shared_ptr<RouteComponent> routeComp = std::make_shared<RouteComponent>(scrollArea.get(), route, gameMap, building);

            routeComp->setY(yoffset);
            scrollArea->addObject(routeComp);
            yoffset += routeComp->getHeight() + 10;
        }
    }

};