#include "routecomponent.h"
#include "engine/ui/Checkbox.h"
#include "services/productservice.h"
#include "translate.h"
#include "world/buildings/StorageComponent.h"
#include "world/gamemap.h"
namespace UI
{
    RouteComponent::RouteComponent(UI::Object *parent, const std::shared_ptr<world::buildings::TransportRoute> &route, GameMap *gameMap, const std::shared_ptr<world::Building> &parentBuilding, const std::shared_ptr<world::Company> &player)
        : UI::Container(), UI::Object(parent), route(route), gameMap(gameMap), parentBuilding(parentBuilding), player(player)
    {

        setHeight(70);
        setWidth(parent->displayRect().width - 10);
        setHeight(100);
        setObjectName("RouteComponent");
        closeButton = std::make_shared<UI::Button>(this);
        closeButton->setX(parent->displayRect().width - 50);
        closeButton->setY(10);
        std::string iconFontName = getTheme()->getStyleText(this, UI::StyleType::IconFontName);
        int iconFontSize = getTheme()->getStyleInt(this, UI::StyleType::IconFontSize);

        closeButton->setFont(iconFontName, iconFontSize);
        closeButton->setLabel("\uf00d");
        closeButton->setBorderless(true);

        addObject(closeButton);
        initUI();
    }

    void RouteComponent::render(core::Renderer *pRender)
    {
        UI::Container::render(pRender);
    }

    bool RouteComponent::handleEvents(core::Input *pInput)
    {
        return UI::Container::handleEvents(pInput);
    }

    void RouteComponent::fillProductListByBuilding(std::shared_ptr<world::Building> &building)
    {
        productList->clearElements();

        auto storage = building->getComponent<world::buildings::StorageComponent>("StorageComponent");

        for (auto &productName : storage->getStoredProducts())
        {
            auto product = services::ProductService::Instance().getProductByName(productName);
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
                                            return var->getLocalisedName(); });

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

        auto productionBuildings = gameMap->findStorageBuildings(parentBuilding, player);
        int i = 0;
        for (auto &building : productionBuildings)
        {
            startBuildings->addElement(building);
            finishBuildings->addElement(building);
            if (route->startBuilding == nullptr)
            {
                route->startBuilding = building;
                route->startBuildingName = building->getDisplayName();
            }
            i++;
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
            if (route->product == nullptr)
            {

                if (route->startBuilding != nullptr && route->startBuilding->getProducts().size() > 0)
                {
                    route->product = route->startBuilding->getProducts()[0];
                }
            }
        }
        else if (productionBuildings.size() > 0)
        {
            route->startBuilding = productionBuildings[0];
            route->startBuildingName = productionBuildings[0]->getDisplayName();
            if (route->startBuilding != nullptr && route->startBuilding->getProducts().size() > 0)
            {
                route->product = route->startBuilding->getProducts()[0];
            }
        }

        startBuildings->connect("valueChanged", [&](std::shared_ptr<world::Building> b)
                                {
                                    route->startBuilding = b;
                                    route->startBuildingName = b->getDisplayName();
                                    fillProductListByBuilding(b);
                                    if (route->startBuilding != nullptr && route->startBuilding->getProducts().size() > 0)
                                    {
                                        route->product = route->startBuilding->getProducts()[0];
                                    } });

        finishBuildings->setElementFunction(elemFunction);
        finishBuildings->connect("valueChanged", [&](std::shared_ptr<world::Building> b)
                                 { route->endBuilding = b;
                                 route->endBuildingName = b->getDisplayName(); });
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
