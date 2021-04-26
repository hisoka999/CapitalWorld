#include "buildwindow.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#include <engine/ui/iconbutton.h>
#include <iostream>

namespace UI
{
    BuildWindow::BuildWindow(int x, int y)
        : UI::Window(x, y, 180, 300)
    {
        currentAction = world::BuildAction::None;
        backgroundTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "BuildMenu.png"));
        setTitle("Build");
        setPos(x, y);
        setFont("fonts/Audiowide-Regular.ttf", 14);

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
        int yPos = 10;
        int iconSize = 32;
        int offset = 40;
        SDL_Color defaultColor = {255, 255, 255, 255};
        SDL_Color hoverColor = {0xcd, 0xcd, 0xcd, 0xff};
        SDL_Color clickColor = {0x33, 0x33, 0x33, 0xff};
        auto farmButton = std::make_shared<UI::IconButton>(this);
        farmButton->setLabel("Farm");
        farmButton->setIconText("\uf4d8");

        farmButton->setHoverColor(hoverColor);
        farmButton->setColor(defaultColor);
        farmButton->setPos(xPos, yPos);
        farmButton->setStaticWidth(120);
        farmButton->connect("buttonClick", [&]() {
            currentAction = world::BuildAction::Farm;
        });
        addObject(farmButton);

        yPos += offset;
        auto factoryButton = std::make_shared<UI::IconButton>(this);
        factoryButton->setIconText("\uf472");
        factoryButton->setLabel("Factory");
        factoryButton->setPos(xPos, yPos);
        factoryButton->setStaticWidth(120);
        //factoryButton->setClickColor(clickColor);
        factoryButton->setHoverColor(hoverColor);
        factoryButton->setColor(defaultColor);
        addObject(factoryButton);
        factoryButton->connect("buttonClick", [&]() {
            currentAction = world::BuildAction::Factory;
        });
        yPos += offset;

        auto shopButton = std::make_shared<UI::IconButton>(this);
        shopButton->setIconText("\uf54e");
        shopButton->setLabel("Shop");
        shopButton->setStaticWidth(120);
        //shopButton->setClickColor(clickColor);
        shopButton->setHoverColor(hoverColor);
        shopButton->setColor(defaultColor);
        shopButton->setPos(xPos, yPos);
        shopButton->disable();
        addObject(shopButton);
        yPos += offset;

        auto transportButton = std::make_shared<UI::IconButton>(this);
        transportButton->setIconText("\uf0d1");
        transportButton->setPos(xPos, yPos);
        transportButton->setLabel("Transport");
        //transportButton->setClickColor(clickColor);
        transportButton->setHoverColor(hoverColor);
        transportButton->setColor(defaultColor);
        transportButton->setStaticWidth(120);
        transportButton->disable();
        addObject(transportButton);

        yPos += offset;

        auto streetButton = std::make_shared<UI::IconButton>(this);
        streetButton->setIconText("\uf018");
        streetButton->setLabel("Street");
        streetButton->setPos(xPos, yPos);
        //destroyButton->setClickColor(clickColor);
        streetButton->setHoverColor(hoverColor);
        streetButton->setColor(defaultColor);
        streetButton->setStaticWidth(120);
        streetButton->disable();
        addObject(streetButton);
        streetButton->connect("buttonClick", [&]() {
            currentAction = world::BuildAction::Street;
        });

        yPos += offset;

        auto destroyButton = std::make_shared<UI::IconButton>(this);
        destroyButton->setIconText("\uf2ed");
        destroyButton->setLabel("Destroy");
        destroyButton->setPos(xPos, yPos);
        //destroyButton->setClickColor(clickColor);
        destroyButton->setHoverColor(hoverColor);
        destroyButton->setColor(defaultColor);
        destroyButton->setStaticWidth(120);
        addObject(destroyButton);
        destroyButton->connect("buttonClick", [&]() {
            currentAction = world::BuildAction::Destroy;
        });
    }

    void BuildWindow::render(core::Renderer *pRender)
    {

        UI::Window::render(pRender);
    }
    void BuildWindow::handleEvents(core::Input *pInput)
    {
        UI::Window::handleEvents(pInput);
    }

}
