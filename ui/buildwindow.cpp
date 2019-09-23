#include "buildwindow.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#include <engine/ui/ImageButton.h>
#include <iostream>

namespace UI
{
BuildWindow::BuildWindow(int x, int y)
    :UI::Container(),UI::Object()
{
    currentAction = world::BuildAction::None;
    backgroundTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images","build_ui.png"));

    setPos(x,y-backgroundTexture->getHeight()/2);

    setWidth(backgroundTexture->getWidth());
    setHeight(backgroundTexture->getHeight());
    initUI();

}
BuildWindow::~BuildWindow()
{

}
void BuildWindow::initUI()
{
    int xPos = 15;
    int yPos = 50;
    auto farmButton = new UI::ImageButton(this,50,50,0,0,false);
    farmButton->loadImage(utils::os::combine("images","icons","tractor.png"));
    farmButton->setPos(xPos,yPos);
    farmButton->connect("buttonClick",[&](){
       currentAction = world::BuildAction::Farm;
    });
    addObject(farmButton);

    yPos+=70;
    auto factoryButton = new UI::ImageButton(this,50,50,0,0,false);
    factoryButton->loadImage(utils::os::combine("images","icons","factory.png"));
    factoryButton->setPos(xPos,yPos);
    addObject(factoryButton);
    factoryButton->connect("buttonClick",[&](){
       currentAction = world::BuildAction::Factory;
    });
    yPos+=70;

    auto shopButton = new UI::ImageButton(this,50,50,0,0,false);
    shopButton->loadImage(utils::os::combine("images","icons","places.png"));
    shopButton->setPos(xPos,yPos);
    addObject(shopButton);
    yPos+=70;

    auto transportButton = new UI::ImageButton(this,50,50,0,0,false);
    transportButton->loadImage(utils::os::combine("images","icons","lorry.png"));
    transportButton->setPos(xPos,yPos);
    addObject(transportButton);
    yPos+=70;

    auto destroyButton = new UI::ImageButton(this,50,50,0,0,false);
    destroyButton->loadImage(utils::os::combine("images","icons","caterpillar.png"));
    destroyButton->setPos(xPos,yPos);
    addObject(transportButton);
    destroyButton->connect("buttonClick",[&](){
       currentAction = world::BuildAction::Destroy;
    });

}

void BuildWindow::render(core::Renderer *pRender, graphics::Texture *pTexture)
{
    backgroundTexture->render(pRender,this->getX(),getY());
    UI::Container::render(pRender,pTexture);
}
void BuildWindow::handleEvents(core::Input *pInput)
{
    UI::Container::handleEvents(pInput);
}


}
