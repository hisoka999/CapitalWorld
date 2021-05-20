#include "buildingwindow.h"
#include <engine/ui/Label.h>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#include "../ui/farmproductiontab.h"
#include "../ui/factoryproductiontab.h"
#include "../ui/routestab.h"
#include "../ui/storagetab.h"

namespace UI
{

    BuildingWindow::BuildingWindow(int x, int y)
        : UI::Window(x, y, 840, 520)
    {
        setTitle("Show Building");
        //auto baseFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts","arial.ttf"),12);
        //setFont(baseFont.get());
        tabBar = std::make_shared<UI::TabBar>(this);
        tabBar->setPos(0, 0);
        tabBar->setWidth(740 - 10);
        tabBar->setHeight(520 - 50);
        addObject(tabBar);
        std::shared_ptr<UI::Tab> infoTab = std::make_shared<UI::Tab>(tabBar.get(), "Info");
        tabBar->addTab(infoTab);

        //fill info tab
        std::shared_ptr<UI::Label> labelName = std::make_shared<UI::Label>("Type: ", infoTab.get());
        labelName->setPos(5, 5);
        infoTab->addObject(labelName);

        std::shared_ptr<UI::Label> labelOwner = std::make_shared<UI::Label>("Owner: ", infoTab.get());
        labelOwner->setPos(5, 25);
        infoTab->addObject(labelOwner);

        std::shared_ptr<UI::Label> labelGround = std::make_shared<UI::Label>("Ground: ", infoTab.get());
        labelGround->setPos(5, 45);
        infoTab->addObject(labelGround);

        labelTypeValue = std::make_shared<UI::Label>(infoTab.get());
        labelTypeValue->setPos(100, 5);
        infoTab->addObject(labelTypeValue);
        labelOwnerValue = std::make_shared<UI::Label>(infoTab.get());
        labelOwnerValue->setPos(100, 25);
        infoTab->addObject(labelOwnerValue);
        labelGroundValue = std::make_shared<UI::Label>(infoTab.get());
        labelGroundValue->setPos(100, 45);
        infoTab->addObject(labelGroundValue);
    }
    void BuildingWindow::open(std::shared_ptr<world::Building> building, std::shared_ptr<world::Company> company, TileType tile, GameMap *gameMap)
    {
        this->building = building;
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

            labelGroundValue->setText("Grass");
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
                productionTab = std::make_shared<UI::RoutesTab>(tabBar.get(), building, gameMap);
            }
            if (productionTab != nullptr)
            {
                tabBar->addTab(productionTab);
                storageTab = std::make_shared<UI::StorageTab>(tabBar.get(), building);
                tabBar->addTab(storageTab);
            }
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
            labelGroundValue->setText("TODO");
        }
        this->setVisible(true);
    }
}
