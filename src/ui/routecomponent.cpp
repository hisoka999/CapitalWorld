#include "routecomponent.h"
#include "world/gamemap.h"
#include "translate.h"
#include "engine/ui/Checkbox.h"

namespace UI
{
    RouteComponent::RouteComponent(UI::Object *parent, const std::shared_ptr<world::buildings::TransportRoute> &route, GameMap *gameMap, const std::shared_ptr<world::Building> &parentBuilding)
        : UI::Object(parent), UI::Container(), route(route), gameMap(gameMap), parentBuilding(parentBuilding)
    {
        initUI();
        setHeight(70);
    }

    void RouteComponent::render(core::Renderer *pRender)
    {
        UI::Container::render(pRender);
    }

    void RouteComponent::handleEvents(core::Input *pInput)
    {
        UI::Container::handleEvents(pInput);
    }

    void RouteComponent::fillProductListByBuilding(std::shared_ptr<world::Building> &building)
    {
        productList->clearElements();
        for (auto &product : building->getProducts())
        {
            productList->addElement(product);
        }
    }

    void RouteComponent::initUI()
    {
        std::shared_ptr<UI::ComboBox<std::shared_ptr<world::Building>>> startBuildings = std::make_shared<UI::ComboBox<std::shared_ptr<world::Building>>>(this);
        std::shared_ptr<UI::ComboBox<std::shared_ptr<world::Building>>> finishBuildings = std::make_shared<UI::ComboBox<std::shared_ptr<world::Building>>>(this);
        productList = std::make_shared<UI::ComboBox<std::shared_ptr<Product>>>(this);
        productList->setElementFunction([&](std::shared_ptr<Product> var)
                                        {
                                            if (var == nullptr)
                                                return std::string("empty");
                                            return var->getName();
                                        });

        productList->connect("valueChanged", [&](std::shared_ptr<Product> b)
                             { route->product = b; });

        auto startLabel = std::make_shared<UI::Label>(_("Start: "), this);
        auto finishLabel = std::make_shared<UI::Label>(_("Target: "), this);
        auto productLabel = std::make_shared<UI::Label>(_("Product:"), this);
        auto activeCheckbox = std::make_shared<UI::Checkbox>(this);
        activeCheckbox->setText(_("is active"));
        activeCheckbox->setChecked(route->active);
        activeCheckbox->connect("checkboxChanged", [&](bool val)
                                { route->active = val; });

        auto productionBuildings = gameMap->findProductionBuildings(parentBuilding);
        for (auto &building : productionBuildings)
        {
            startBuildings->addElement(building);
            finishBuildings->addElement(building);
        }

        auto elemFunction = [&](std::shared_ptr<world::Building> var)
        {
            if (var == nullptr)
                return std::string("empty");
            return var->getDisplayName();
        };
        startBuildings->setElementFunction(elemFunction);
        if (route->startBuilding != nullptr)
        {
            startBuildings->setSelectionByText(route->startBuilding);
            fillProductListByBuilding(route->startBuilding);
        }
        startBuildings->connect("valueChanged", [&](std::shared_ptr<world::Building> b)
                                {
                                    route->startBuilding = b;
                                    fillProductListByBuilding(b);
                                    if (route->startBuilding != nullptr && route->startBuilding->getProducts().size() > 0)
                                    {
                                        route->product = route->startBuilding->getProducts()[0];
                                    }
                                });

        finishBuildings->setElementFunction(elemFunction);
        finishBuildings->connect("valueChanged", [&](std::shared_ptr<world::Building> b)
                                 { route->endBuilding = b; });
        if (route->endBuilding != nullptr)
        {
            finishBuildings->setSelectionByText(route->endBuilding);
        }
        else if (productionBuildings.size() > 0)
        {
            route->endBuilding = productionBuildings[0];
        }

        if (route->product != nullptr)
        {
            productList->setSelectionByText(route->product);
        }
        startBuildings->setPos(50, 10);
        startBuildings->setWidth(140);
        finishBuildings->setPos(50, 40);
        finishBuildings->setWidth(140);
        productList->setPos(270, 10);
        productList->setWidth(140);
        startLabel->setPos(5, 15);
        finishLabel->setPos(5, 45);
        productLabel->setPos(210, 15);
        activeCheckbox->setPos(210, 40);
        addObject(startBuildings);
        addObject(finishBuildings);
        addObject(startLabel);
        addObject(finishLabel);
        addObject(productList);
        addObject(productLabel);
        addObject(activeCheckbox);
    }

};
