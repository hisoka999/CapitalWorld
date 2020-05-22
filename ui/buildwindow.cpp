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
    backgroundTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images","BuildMenu.png"));


    setPos(x,y);

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
    int iconSize = 32;
    int offset = 40;
    SDL_Color defaultColor ={255,255,255,255};
    SDL_Color hoverColor = {0xcd,0xcd,0xcd,0xff};
    SDL_Color clickColor = {0x33,0x33,0x33,0xff};
    auto farmButton = new UI::ImageButton(this,iconSize,iconSize,0,0,true);
    farmButton->setText("Farm");
    farmButton->setClickColor(clickColor);
    farmButton->setHoverColor(hoverColor);
    farmButton->setDefaultColor(defaultColor);
    farmButton->loadImage(utils::os::combine("images","icons","tractor.png"));
    farmButton->setPos(xPos,yPos);
    farmButton->connect("buttonClick",[&](){
       currentAction = world::BuildAction::Farm;
    });
    addObject(farmButton);

    yPos+=offset;
    auto factoryButton = new UI::ImageButton(this,iconSize,iconSize,0,0,false);
    factoryButton->loadImage(utils::os::combine("images","icons","factory.png"));
    factoryButton->setText("Factory");
    factoryButton->setPos(xPos,yPos);
    factoryButton->setClickColor(clickColor);
    factoryButton->setHoverColor(hoverColor);
    farmButton->setDefaultColor(defaultColor);
    addObject(factoryButton);
    factoryButton->connect("buttonClick",[&](){
       currentAction = world::BuildAction::Factory;
    });
    yPos+=offset;

    auto shopButton = new UI::ImageButton(this,iconSize,iconSize,0,0,false);
    shopButton->loadImage(utils::os::combine("images","icons","places.png"));
    shopButton->setText("Shop");
    shopButton->setClickColor(clickColor);
    shopButton->setHoverColor(hoverColor);
    shopButton->setDefaultColor(defaultColor);
    shopButton->setPos(xPos,yPos);
    addObject(shopButton);
    yPos+=offset;

    auto transportButton = new UI::ImageButton(this,iconSize,iconSize,0,0,false);
    transportButton->loadImage(utils::os::combine("images","icons","lorry.png"));
    transportButton->setPos(xPos,yPos);
    transportButton->setText("Transport");
    transportButton->setClickColor(clickColor);
    transportButton->setHoverColor(hoverColor);
    transportButton->setDefaultColor(defaultColor);
    addObject(transportButton);
    yPos+=offset;

    auto destroyButton = new UI::ImageButton(this,iconSize,iconSize,0,0,false);
    destroyButton->loadImage(utils::os::combine("images","icons","caterpillar.png"));
    destroyButton->setText("Destroy");
    destroyButton->setPos(xPos,yPos);
    destroyButton->setClickColor(clickColor);
    destroyButton->setHoverColor(hoverColor);
    destroyButton->setDefaultColor(defaultColor);
    addObject(destroyButton);
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
