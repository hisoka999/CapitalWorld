#include "worldscene.h"
#include "../world/mapgenerator.h"
#include <chrono>
#include <cmath>
#include <engine/utils/os.h>
#include <engine/utils/string.h>
#include <iostream>
#include <random>

namespace scenes
{

    WorldScene::WorldScene(core::Renderer *pRenderer,
                           core::SceneManager *pSceneManager)
        : core::Scene(pRenderer), sceneManager(
                                      pSceneManager),
          buildWindow(0, static_cast<int>(pRenderer->getViewPort().height / 2.0f)), buildingWindow(100, 100)
    {
        cursorTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "tiles", "iso_cursor.png"));
        hudTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "ui_base.png"));
        hudFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "arial.ttf"), 16);
        world::MapGenerator gen;
        std::random_device r;

        gameMap = gen.generateMap(100, 100, r());
        cities = gen.getGeneratedCities();

        //gameMap = std::make_shared<GameMap>(100,100);
        mapRenderer = std::make_shared<GameMapRenderer>(gameMap);
        auto playerCompany = std::make_shared<world::Company>("Player Company", 1000000, true);
        gameState = std::make_shared<world::GameState>(playerCompany);
        renderer->setZoomFactor(6);
        buildWindow.setFont(hudFont.get());

        uiTexture.loadTexture(renderer, utils::os::combine("images", "ArkanaLook.png"));

        thread = std::make_unique<UpdateThread>(gameState);
        winMgr->addWindow(&buildingWindow);
    }
    WorldScene::~WorldScene()
    {
        thread->stop();
    }
    std::shared_ptr<world::Building> WorldScene::createBuilding(world::BuildAction action)
    {
        std::shared_ptr<world::Building> building = nullptr;
        graphics::Rect rect;
        rect.width = static_cast<float>(mapRenderer->getTileWidth());
        rect.height = static_cast<float>(mapRenderer->getTileHeight());
        switch (action)
        {
        case world::BuildAction::Farm:
            building = std::make_shared<world::Building>("Farm", "A farm", 10000, world::BuildingType::Farm);
            rect.x = 754;
            rect.y = 236;
            building->setOffset(0, -4);
            building->setSourceRect(rect);
            break;

        case world::BuildAction::Factory:
            building = std::make_shared<world::Building>("Factory", "A factory", 20000, world::BuildingType::Factory, 3, 3);
            rect.x = 14;
            rect.y = 1878;
            rect.width = 184;
            rect.height = 162;
            building->setOffset(0, -106);
            building->setSourceRect(rect);

            break;

        default:
            break;
        }
        return building;
    }

    void WorldScene::renderHUD()
    {
        int y = 0;
        int height = hudTexture->getHeight() * 720 / renderer->getViewPort().height / 2;
        buildWindow.setPos(0, height);
        hudTexture->renderResized(renderer, 0, y, renderer->getViewPort().width, height);

        //render hud text
        SDL_Color color = {0xef, 0xef, 0xef, 0xff};
        y += 5;
        hudFont->render(renderer, "Cash: ", color, 318, y);
        auto playerCompany = gameState->getPlayer();
        hudFont->render(renderer, utils::string_format("%'.2f €", playerCompany->getCash()), color, 360, y);

        std::time_t tmpTime = std::chrono::system_clock::to_time_t(
            gameState->getTime());

        char date_time_format[] = "%d.%m.%Y";

        char time_str[100];

        std::strftime(time_str, 100, date_time_format,
                      std::localtime(&tmpTime));

        hudFont->render(renderer, std::string(time_str), color, 180, y);
        hudFont->render(renderer, "Profit:", color, 540, y);
        hudFont->render(renderer, utils::string_format("%'.2f €", playerCompany->getProfit()), color, 590, y);
        //render icons
        buildWindow.render(renderer);
    }

    void WorldScene::render()
    {

        size_t width = gameMap->getHeight() * mapRenderer->getTileWidth();
        size_t height = gameMap->getWidth() * mapRenderer->getTileHeight() / 2;

        mapRenderer->render(renderer);
        for (auto city : cities)
        {
            city->renderCity(renderer);
        }
        float cursorX = (cursorPosition.getX() * mapRenderer->getTileWidth() / 2.f);
        float cursorY = (cursorPosition.getY() * mapRenderer->getTileHeight());
        utils::Vector2 pos = gameMap->twoDToIso(utils::Vector2(cursorX, cursorY));
        auto camera = renderer->getMainCamera();
        int xPos = (pos.getX() * renderer->getZoomFactor()) - camera->getX();

        float tileYOffset = mapRenderer->getTileYOffset(gameMap->getTile(cursorPosition.getX(), cursorPosition.getY()), cursorPosition.getX(), cursorPosition.getY());

        int yPos = ((pos.getY() - tileYOffset) * renderer->getZoomFactor()) - (camera->getY());

        cursorTexture->renderResized(renderer, xPos, yPos, cursorTexture->getWidth() * renderer->getZoomFactor(), cursorTexture->getHeight() * renderer->getZoomFactor());

        renderHUD();

        winMgr->render(renderer);
    }

    void WorldScene::handleEvents(core::Input *pInput)
    {
        bool mouseIntersectsWindow = buildWindow.displayRect().intersects(pInput->getMousePostion());

        if (!mouseIntersectsWindow && !winMgr->isWindowOpen())
        {
            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                std::cout << "build here" << std::endl;
                auto action = buildWindow.getCurrentAction();
                if (action == world::BuildAction::Destroy)
                {
                    //check if building exists and then destroy it
                    auto building = gameMap->getBuilding2D(cursorPosition);
                    if (building != nullptr)
                    {
                        //remove it

                        gameMap->removeBuilding(building);
                        //get back cash
                        if (gameState->getPlayer()->hasBuilding(building))
                        {
                            gameState->getPlayer()->removeBuilding(building);
                            gameState->getPlayer()->incCash(building->getBuildPrice() * 0.2f);
                        }
                        else
                        {
                            //if no one owns the building you have to pay for destroying it
                            gameState->getPlayer()->incCash(building->getBuildPrice() * -1.0f);
                        }
                    }
                }
                else if (action != world::BuildAction::None)
                {
                    auto building = createBuilding(action);
                    building->setPosition(cursorPosition.getX(), cursorPosition.getY());

                    if (building != nullptr && building->canBuild(gameState->getPlayer()->getCash()) && gameMap->canBuild(building->get2DPosition()))
                    {

                        gameMap->addBuilding(building);
                        gameState->getPlayer()->addBuilding(building);
                        gameState->getPlayer()->incCash(building->getBuildPrice() * -1);
                    }
                }
                else
                {
                    auto building = gameMap->getBuilding2D(cursorPosition);
                    std::shared_ptr<world::Company> company = nullptr;
                    if (gameState->getPlayer()->hasBuilding(building))
                    {
                        company = gameState->getPlayer();
                    }
                    buildingWindow.open(building, company, gameMap->getTile(cursorPosition));
                }
            }
            else if (pInput->isMouseButtonPressed(SDL_BUTTON_RIGHT))
            {
                buildWindow.setCurrentAction(world::BuildAction::None);
            }
            if (pInput->isScrollWheel())
            {
                auto wheelPosition = pInput->getMouseWheelPosition();
                float factor = renderer->getZoomFactor() + (wheelPosition.getY() / 10.f);

                renderer->setZoomFactor(factor);
                std::cout << "factor: " << factor << std::endl;
            }
            if (pInput->isMouseMoving())
            {
                float camX = renderer->getMainCamera()->getX();
                float camY = renderer->getMainCamera()->getY();

                utils::Vector2 pt = gameMap->isoTo2D(pInput->getMousePostion() + utils::Vector2(camX, camY)
                                                     //-utils::Vector2(mapRenderer->getTileWidth()/4.0,mapRenderer->getTileHeight()/4.0)
                )

                    ;

                float x, y = 0.0;

                float tx = pt.getX() / static_cast<float>(mapRenderer->getTileHeight() * renderer->getZoomFactor());
                float ty = pt.getY() / static_cast<float>(mapRenderer->getTileHeight() * renderer->getZoomFactor());

                x = std::floor(tx - 0.5f);
                y = std::floor(ty - 0.5f);

                cursorPosition = utils::Vector2(x, y);
                std::cout << "x: " << cursorPosition.getX() << " y: " << cursorPosition.getY() << std::endl;
            }
        }

        float yOffset = 0;
        float xOffset = 0;
        if (pInput->isKeyDown(SDLK_w))
        {
            yOffset = -1.0f;
        }
        else if (pInput->isKeyDown(SDLK_s))
        {
            yOffset = 1.0f;
        }
        if (pInput->isKeyDown(SDLK_a))
        {
            xOffset = -1.0f;
        }
        else if (pInput->isKeyDown(SDLK_d))
        {
            xOffset = 1.0f;
        }
        renderer->getMainCamera()->move(xOffset * 5.0f, yOffset * 5.0f);

        buildWindow.handleEvents(pInput);
        winMgr->handleInput(pInput);
    }
}
