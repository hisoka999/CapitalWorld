#include "routecomponent.h"
#include "world/gamemap.h"
namespace UI
{
    RouteComponent::RouteComponent(UI::Object *parent, const std::shared_ptr<world::buildings::TransportRoute> &route, GameMap *gameMap, const std::shared_ptr<world::Building> &parentBuilding)
        : UI::Object(parent), UI::Container(), route(route), gameMap(gameMap), parentBuilding(parentBuilding)
    {
        initUI();
    }

    void RouteComponent::render(core::Renderer *pRender)
    {
        UI::Container::render(pRender);
    }

    void RouteComponent::handleEvents(core::Input *pInput)
    {
        UI::Container::handleEvents(pInput);
    }

    void RouteComponent::initUI()
    {
        std::shared_ptr<UI::ComboBox<std::shared_ptr<world::Building>>> startBuildings = std::make_shared<UI::ComboBox<std::shared_ptr<world::Building>>>(this);

        startBuildings->setElementFunction([&](std::shared_ptr<world::Building> var)
                                           {
                                               if (var == nullptr)
                                                   return std::string("empty");
                                               return var->getDisplayName();
                                           });

        auto productionBuildings = gameMap->findProductionBuildings(parentBuilding);
        for (auto &building : productionBuildings)
        {
            startBuildings->addElement(building);
        }

        startBuildings->setPos(50, 50);
        addObject(startBuildings);
    }

};
