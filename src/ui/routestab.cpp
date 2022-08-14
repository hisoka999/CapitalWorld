#include "routestab.h"
#include <future>
#include <translate.h>
namespace UI
{
    RoutesTab::RoutesTab(UI::Object *parent, const std::shared_ptr<world::Building> &building, GameMap *gameMap, const std::shared_ptr<world::Company> &player) : UI::Tab(parent, _("Transport routes")), gameMap(gameMap), player(player)
    {
        setWidth(parent->getWidth() - 100);
        setHeight(parent->getHeight() - 10);
        setBuilding(building);
    }

    void RoutesTab::setBuilding(const std::shared_ptr<world::Building> &building)
    {
        this->building = building;
        initUI();
    }

    void RoutesTab::refresh()
    {
        refreshComponents();
        Tab::refresh();
    }

    void RoutesTab::initUI()
    {
        clear();
        scrollArea = std::make_shared<UI::ScrollArea>(getWidth(), getHeight() - 50, this);
        scrollArea->setPos(0, 0);
        addObject(scrollArea);
        refreshComponents();
        std::shared_ptr<UI::Button> newRouteButton = std::make_shared<UI::Button>(this);
        newRouteButton->setLabel(_("Add Route"));
        newRouteButton->setPos(5, getHeight() - 50);
        newRouteButton->connect(UI::Button::buttonClickCallback(), [&]()
                                {
                                    std::shared_ptr<world::buildings::TransportComponent> transportComp = building->getComponent<world::buildings::TransportComponent>("TransportComponent");

                                    transportComp->addRoute(nullptr, nullptr, nullptr, 0);
                                    needsRefresh(); });
        addObject(newRouteButton);
    }

    void RoutesTab::refreshComponents()
    {
        scrollArea->reset();

        scrollArea->clear();
        int yoffset = 0;
        if (building == nullptr)
            return;
        std::shared_ptr<world::buildings::TransportComponent> transportComp = building->getComponent<world::buildings::TransportComponent>("TransportComponent");
        size_t i = 0;
        for (auto &route : transportComp->getAllRoutes())
        {
            std::shared_ptr<RouteComponent> routeComp = std::make_shared<RouteComponent>(scrollArea.get(), route, gameMap, building, player);
            const size_t value = i;
            routeComp->closeButton->connect(UI::Button::buttonClickCallback(), [=]()
                                            {
                                                transportComp->removeRoute(value);
                                                needsRefresh(); });
            routeComp->setY(yoffset);
            scrollArea->addObject(routeComp);
            yoffset += routeComp->getHeight() + 10;
            i++;
        }
    }

};