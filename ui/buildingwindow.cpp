#include "buildingwindow.h"
#include <engine/ui/Label.h>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#include <ui/farmproductiontab.h>

namespace UI {

BuildingWindow::BuildingWindow(int x,int y)
    :UI::Window(x,y,600,400),tabBar(this)
{
    setTitle("Show Building");
    //auto baseFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts","arial.ttf"),12);
    //setFont(baseFont.get());

    tabBar.setPos(0,0);
    tabBar.setWidth(600-10);
    tabBar.setHeight(400 -50);
    addObject(&tabBar);
    std::shared_ptr<UI::Tab> infoTab = std::make_shared<UI::Tab>(&tabBar,"Info");
    tabBar.addTab(infoTab);

    //fill info tab
    UI::Label* labelName = new UI::Label("Type: ",infoTab.get());
    labelName->setPos(5,5);
    infoTab->addObject(labelName);

    UI::Label* labelOwner = new UI::Label("Owner: ",infoTab.get());
    labelOwner->setPos(5,25);
    infoTab->addObject(labelOwner);

    UI::Label* labelGround = new UI::Label("Ground: ",infoTab.get());
    labelGround->setPos(5,45);
    infoTab->addObject(labelGround);

    labelTypeValue = new UI::Label(infoTab.get());
    labelTypeValue->setPos(100,5);
    infoTab->addObject(labelTypeValue);
    labelOwnerValue = new UI::Label(infoTab.get());
    labelOwnerValue->setPos(100,25);
    infoTab->addObject(labelOwnerValue);
    labelGroundValue = new UI::Label(infoTab.get());
    labelGroundValue->setPos(100,45);
    infoTab->addObject(labelGroundValue);



}
void BuildingWindow::open(std::shared_ptr<world::Building> building,TileType tile)
{
    this->building = building;
    if(building != nullptr)
    {
        labelTypeValue->setText(building->getName());
        labelOwnerValue->setText("TODO");
        labelGroundValue->setText("TODO");
        tabBar.removeTab(productionTab);
        //recreate tab based on Building type
        productionTab = std::make_shared<UI::FarmProductionTab>(this);
        tabBar.addTab(productionTab);
    }
    else
    {
        labelTypeValue->setText("");
        labelOwnerValue->setText("");
        labelGroundValue->setText("TODO");
    }
    this->setVisible(true);

}
}
