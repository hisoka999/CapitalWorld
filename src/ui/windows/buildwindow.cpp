#include "buildwindow.h"
#include "translate.h"
#include "world/buildings/street.h"
#include <engine/graphics/TextureManager.h>
#include <engine/ui/iconbutton.h>
#include <engine/utils/os.h>
#include <iostream>
#include <engine/ui/StringHint.h>
namespace UI
{
    BuildWindow::BuildWindow(int x, int y, UI::BuildingSelectionWindow *buildingSelectionWindow)
        : UI::Window(0, 0, 180, 320), cursor(nullptr), buildingSelectionWindow(buildingSelectionWindow), selectedBuilding(nullptr)
    {
        setCurrentAction(world::BuildAction::None);
        setPos(x, y);
        setFont("fonts/Audiowide-Regular.ttf", 14);
        setWithoutTitle(true);
        setVisible(true);

        // setWidth(backgroundTexture->getWidth());
        // setHeight(backgroundTexture->getHeight());
        initUI();
    }
    BuildWindow::~BuildWindow()
    {
    }
    void BuildWindow::initUI()
    {
        int xPos = 5;
        int yPos = (getHeight() / 2);
        int offset = 40;
        SDL_Color defaultColor = {255, 255, 255, 255};
        SDL_Color hoverColor = {0xcd, 0xcd, 0xcd, 0xff};
        auto farmButton = std::make_shared<UI::IconButton>(this);
        farmButton->setLabel(_("Resources"));
        farmButton->setIconText("\uf4d8");
        // farmButton->setBorderless(true);

        farmButton->setHoverColor(hoverColor);
        farmButton->setColor(defaultColor);
        farmButton->setPos(xPos, yPos);
        farmButton->setStaticWidth(120);
        farmButton->setHeight(35);
        farmButton->connect("buttonClick", [&]()
                            {
                                setCurrentAction(world::BuildAction::Build);
                                currentBuildingType = world::BuildingType::Resource;
                                buildingSelectionWindow->setBuildingType(currentBuildingType);
                                buildingSelectionWindow->setVisible(true); });
        addObject(farmButton);

        yPos += offset;
        auto factoryButton = std::make_shared<UI::IconButton>(this);
        factoryButton->setIconText("\uf472");
        factoryButton->setLabel(_("Factory"));
        factoryButton->setPos(xPos, yPos);
        factoryButton->setStaticWidth(120);
        factoryButton->setHeight(35);
        factoryButton->setHint(std::make_shared<UI::StringHint>());
        factoryButton->getHint()->setHintText(_("Factory"));
        // factoryButton->setClickColor(clickColor);
        factoryButton->setHoverColor(hoverColor);
        factoryButton->setColor(defaultColor);
        // factoryButton->setBorderless(true);
        addObject(factoryButton);
        factoryButton->connect("buttonClick", [&]()
                               {
                                   setCurrentAction(world::BuildAction::Build);
                                   currentBuildingType = world::BuildingType::Factory;
                                   buildingSelectionWindow->setBuildingType(currentBuildingType);
                                   buildingSelectionWindow->setVisible(true); });
        yPos += offset;

        auto shopButton = std::make_shared<UI::IconButton>(this);
        shopButton->setIconText("\uf54e");
        shopButton->setLabel("Shop");
        shopButton->setStaticWidth(120);
        // shopButton->setClickColor(clickColor);
        shopButton->setHoverColor(hoverColor);
        shopButton->setColor(defaultColor);
        shopButton->setPos(xPos, yPos);

        shopButton->connect("buttonClick", [&]()
                            {
                                setCurrentAction(world::BuildAction::Build);
                                currentBuildingType = world::BuildingType::Shop;
                                buildingSelectionWindow->setBuildingType(currentBuildingType);
                                buildingSelectionWindow->setVisible(true); });

        addObject(shopButton);
        yPos += offset;

        auto transportButton = std::make_shared<UI::IconButton>(this);
        transportButton->setIconText("\uf0d1");
        transportButton->setPos(xPos, yPos);
        transportButton->setLabel(_("Transport"));
        // transportButton->setClickColor(clickColor);
        transportButton->setHoverColor(hoverColor);
        transportButton->setColor(defaultColor);
        transportButton->setStaticWidth(120);
        transportButton->connect("buttonClick", [&]()
                                 {
                                     setCurrentAction(world::BuildAction::Build);
                                     currentBuildingType = world::BuildingType::Transport;
                                     buildingSelectionWindow->setBuildingType(currentBuildingType);
                                     buildingSelectionWindow->setVisible(true); });

        addObject(transportButton);
        yPos += offset;

        auto otherButton = std::make_shared<UI::IconButton>(this);
        otherButton->setIconText("\uf0d1");
        otherButton->setPos(xPos, yPos);
        otherButton->setLabel(_("Other"));
        // transportButton->setClickColor(clickColor);
        otherButton->setHoverColor(hoverColor);
        otherButton->setColor(defaultColor);
        otherButton->setStaticWidth(120);
        otherButton->connect("buttonClick", [&]()
                             {
                                 setCurrentAction(world::BuildAction::Build);
                                 currentBuildingType = world::BuildingType::Other;
                                 buildingSelectionWindow->setBuildingType(currentBuildingType);
                                 buildingSelectionWindow->setVisible(true); });

        addObject(otherButton);
        yPos += offset;

        auto streetButton = std::make_shared<UI::IconButton>(this);
        streetButton->setIconText("\uf018");
        streetButton->setLabel(_("Street"));
        streetButton->setPos(xPos, yPos);
        // destroyButton->setClickColor(clickColor);
        streetButton->setHoverColor(hoverColor);
        streetButton->setColor(defaultColor);
        streetButton->setStaticWidth(120);

        addObject(streetButton);
        streetButton->connect("buttonClick", [&]()
                              {
                                  setCurrentAction(world::BuildAction::Build);
                                  currentBuildingType = world::BuildingType::Street;
                                  auto building = std::make_shared<world::buildings::Street>();
                                  buildingSelectionWindow->setSelectedBuilding(building);
                                  buildingSelectionWindow->setVisible(false); });

        yPos += offset;

        auto destroyButton = std::make_shared<UI::IconButton>(this);
        destroyButton->setIconText("\uf2ed");
        destroyButton->setLabel(_("Destroy"));
        destroyButton->setPos(xPos, yPos);
        // destroyButton->setClickColor(clickColor);
        destroyButton->setHoverColor(hoverColor);
        destroyButton->setColor(defaultColor);
        destroyButton->setStaticWidth(120);
        addObject(destroyButton);
        destroyButton->connect("buttonClick", [&]()
                               {
                                   setCurrentAction(world::BuildAction::Destroy);
                                   buildingSelectionWindow->setSelectedBuilding(nullptr);
                                   buildingSelectionWindow->setVisible(false); });
    }

    void BuildWindow::render(core::Renderer *pRender)
    {
        if (m_lastHeight != pRender->getViewPort().height)
        {
            m_lastHeight = pRender->getViewPort().height;
            clear();
            initUI();
        }
        UI::Window::render(pRender);
    }
    bool BuildWindow::handleEvents(core::Input *pInput)
    {
        return UI::Window::handleEvents(pInput);
    }

    void BuildWindow::postRender(core::Renderer *pRender)
    {
        UI::Container::postRender(pRender);
    }

    void BuildWindow::setCurrentAction(world::BuildAction action)
    {
        currentAction = action;

        if (cursor != nullptr)
            SDL_FreeCursor(cursor);
        switch (action)
        {
        case world::BuildAction::None:
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

            break;
        case world::BuildAction::Destroy:
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
            break;
        default:
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
            break;
        }
        SDL_SetCursor(cursor);
    }

    std::shared_ptr<world::Building> BuildWindow::getSelectedBuilding()
    {
        return selectedBuilding;
    }

}
