#include "buildingwindow.h"
#include <engine/ui/Label.h>
#include <engine/ui/layout/GridLayout.h>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#include "ui/farmproductiontab.h"
#include "ui/factoryproductiontab.h"
#include "ui/routestab.h"
#include "ui/storagetab.h"
#include "ui/SalesTab.h"
#include "translate.h"

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

        //fill info tab
        std::shared_ptr<UI::Label> labelName = std::make_shared<UI::Label>(_("Type: "), infoTab.get());
        labelName->setPos(5, 5);
        infoTab->addObject(labelName);
        labelTypeValue = std::make_shared<UI::Label>(infoTab.get());
        labelTypeValue->setPos(100, 5);
        infoTab->addObject(labelTypeValue);

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
    void BuildingWindow::open(std::shared_ptr<world::Building> building, std::shared_ptr<world::Company> company, TileType tile, GameMap *gameMap)
    {
        this->building = building;
        labelGroundValue->setText(tileTypeToString(tile));
        for (auto &item : optionalItems)
        {
            infoTab->removeObject(item);
        }
        optionalItems.clear();

        if (building != nullptr)
        {
            labelTypeValue->setText(building->getDisplayName());
            if (company != nullptr)
            {
                labelOwnerValue->setText(company->getName());
            }
            else
            {
                labelOwnerValue->setText("");
            }

            tabBar->removeTab(productionTab);
            tabBar->removeTab(storageTab);
            //recreate tab based on Building type
            productionTab = nullptr;
            storageTab = nullptr;
            switch (building->getType())
            {
            case world::BuildingType::Farm:
                productionTab = std::make_shared<UI::FarmProductionTab>(tabBar.get(), building);
                break;
            case world::BuildingType::Factory:
                productionTab = std::make_shared<UI::FactoryProductionTab>(tabBar.get(), building);
                break;
            case world::BuildingType::Transport:
                productionTab = std::make_shared<UI::RoutesTab>(tabBar.get(), building, gameMap, company);
                break;
            case world::BuildingType::Shop:
                productionTab = std::make_shared<UI::SalesTab>(tabBar.get(), building);
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
            labelTypeValue->setText("");
            labelOwnerValue->setText("");
        }
        this->setVisible(true);
    }
}
