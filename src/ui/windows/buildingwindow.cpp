#include "buildingwindow.h"
#include "translate.h"
#include "ui/SalesTab.h"
#include "ui/WorkerTab.h"
#include "ui/factoryproductiontab.h"
#include "ui/resourceproductiontab.h"
#include "ui/routestab.h"
#include "ui/storagetab.h"
#include <engine/graphics/TextureManager.h>
#include <engine/ui/Label.h>
#include <engine/ui/layout/GridLayout.h>
#include <engine/utils/os.h>
#include <magic_enum.hpp>

namespace UI
{

    BuildingWindow::BuildingWindow(int x, int y)
        : UI::Window(x, y, 840, 520)
    {
        setTitle(_("Show Building"));

        tabBar = std::make_shared<UI::TabBar>(this);
        tabBar->setPos(0, 0);
        tabBar->setWidth(740 - 10);
        tabBar->setHeight(520 - 50);
        addObject(tabBar);
        infoTab = std::make_shared<UI::Tab>(tabBar.get(), "Info");
        tabBar->addTab(infoTab);
        layout = std::make_shared<UI::layout::GridLayout>(infoTab.get(), 2);

        // fill info tab
        std::shared_ptr<UI::Label> labelName = std::make_shared<UI::Label>(_("Type: "), infoTab.get());
        labelName->setPos(5, 5);
        infoTab->addObject(labelName);
        labelTypeValue = std::make_shared<UI::Label>(infoTab.get());
        labelTypeValue->setPos(100, 5);
        infoTab->addObject(labelTypeValue);

        std::shared_ptr<UI::Label> labelNameRes = std::make_shared<UI::Label>(_("Resource: "), infoTab.get());
        labelNameRes->setPos(5, 5);
        infoTab->addObject(labelNameRes);
        labelResourceValue = std::make_shared<UI::Label>(infoTab.get());
        labelResourceValue->setPos(100, 5);
        infoTab->addObject(labelResourceValue);

        std::shared_ptr<UI::Label> labelOwner = std::make_shared<UI::Label>(_("Owner: "), infoTab.get());
        labelOwner->setPos(5, 25);
        infoTab->addObject(labelOwner);
        labelOwnerValue = std::make_shared<UI::Label>(infoTab.get());
        labelOwnerValue->setPos(100, 25);
        infoTab->addObject(labelOwnerValue);

        std::shared_ptr<UI::Label> labelGround = std::make_shared<UI::Label>(_("Ground: "), infoTab.get());
        labelGround->setPos(5, 45);
        infoTab->addObject(labelGround);

        labelGroundValue = std::make_shared<UI::Label>(infoTab.get());
        labelGroundValue->setPos(100, 45);
        infoTab->addObject(labelGroundValue);
        graphics::Rect bounds = tabBar->displayRect();
        bounds.x = 5;
        bounds.y = 5;
        layout->updateLayout(bounds);
    }
    void BuildingWindow::open(std::shared_ptr<world::Building> building, std::shared_ptr<world::GameState> &gameState, utils::Vector2 &position2D, GameMap *gameMap)
    {
        this->building = building;
        TileType tile = gameMap->getTile(position2D);
        labelGroundValue->setText(tileTypeToString(tile));
        world::RawResource rawResource = gameMap->getRawResource(position2D.getX(), position2D.getY());

        labelResourceValue->setText(_(std::string(magic_enum::enum_name(rawResource))));

        for (auto &item : optionalItems)
        {
            infoTab->removeObject(item);
        }
        optionalItems.clear();

        if (building != nullptr)
        {
            labelTypeValue->setText(building->getDisplayName());
            labelOwnerValue->setText("");
            // find owner
            auto owner = gameState->findBuildingOwner(building);
            if (owner != nullptr)
            {
                labelOwnerValue->setText(owner->getName());
            }

            tabBar->removeTab(productionTab);
            tabBar->removeTab(storageTab);
            tabBar->removeTab(workerTab);
            // recreate tab based on Building type
            productionTab = nullptr;
            storageTab = nullptr;
            workerTab = nullptr;
            switch (building->getType())
            {
            case world::BuildingType::Resource:
                productionTab = std::make_shared<UI::ResourceProductionTab>(tabBar.get(), building, rawResource);
                break;
            case world::BuildingType::Factory:
                productionTab = std::make_shared<UI::FactoryProductionTab>(tabBar.get(), building, owner);
                break;
            case world::BuildingType::Transport:
                productionTab = std::make_shared<UI::RoutesTab>(tabBar.get(), building, gameMap, owner);
                break;
            case world::BuildingType::Shop:
                productionTab = std::make_shared<UI::SalesTab>(tabBar.get(), building);
                break;
            default:
                break;
            }
            if (productionTab != nullptr)
            {
                tabBar->addTab(productionTab);
                if (building->hasComponent("StorageComponent"))
                {
                    storageTab = std::make_shared<UI::StorageTab>(tabBar.get(), building);
                    tabBar->addTab(storageTab);
                }
            }

            if (building->hasComponent("WorkerComponent"))
            {
                workerTab = std::make_shared<UI::WorkerTab>(tabBar.get(), building);
                tabBar->addTab(workerTab);
            }

            for (auto data : building->displayData())
            {
                auto labelName = std::make_shared<UI::Label>(data.first, infoTab.get());
                auto labelValue = std::make_shared<UI::Label>(data.second, infoTab.get());
                optionalItems.push_back(labelName);
                optionalItems.push_back(labelValue);
                infoTab->addObject(labelName);
                infoTab->addObject(labelValue);
            }
            graphics::Rect bounds = tabBar->displayRect();
            bounds.x = 5;
            bounds.y = 5;
            layout->updateLayout(bounds);
        }
        else
        {
            if (productionTab != nullptr)
            {
                tabBar->removeTab(productionTab);
                tabBar->removeTab(storageTab);
            }
            if (workerTab != nullptr)
            {
                tabBar->removeTab(workerTab);
            }
            labelTypeValue->setText("");
            labelOwnerValue->setText("");
        }
        this->setVisible(true);
    }
}
