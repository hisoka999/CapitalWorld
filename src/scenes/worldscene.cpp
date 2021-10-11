#include "worldscene.h"
#include "../translate.h"
#include "../world/buildings/street.h"
#include "../world/mapgenerator.h"
#include "services/buildingservice.h"
#include "world/buildings/SalesComponent.h"
#include "world/iso.h"
#include <chrono>
#include <cmath>
#include <engine/utils/os.h>
#include <engine/utils/string.h>
#include <future>
#include <iostream>
#include <random>

namespace scenes
{

    WorldScene::WorldScene(core::Renderer *pRenderer,
                           core::SceneManager *pSceneManager, std::shared_ptr<world::GameState> gameState)

        : core::Scene(pRenderer), sceneManager(
                                      pSceneManager),
          buildingSelectionWindow(200, 100, gameState->getPlayer()), buildWindow(0, static_cast<int>(pRenderer->getViewPort().height / 2.0f), &buildingSelectionWindow), buildingWindow(100, 100), gameState(gameState), optionsWindow(0, 0), researchWindow(gameState)
    {
        cursorTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "cursor.png"));
        hudTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "ui_base.png"));
        hudFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "arial.ttf"), 16);

        //gameMap = std::make_shared<GameMap>(100,100);
        mapRenderer = std::make_shared<GameMapRenderer>(gameState->getGameMap());

        renderer->setZoomFactor(1);
        buildWindow.setFont(hudFont.get());
        buildWindow.setVisible(true);

        uiTexture.loadTexture(renderer, utils::os::combine("images", "ArkanaLook.png"));

        thread = std::make_unique<UpdateThread>(gameState);
        winMgr->addWindow(&buildingWindow);

        hud = std::make_shared<UI::HUDContainer>(thread.get(), gameState, &buildWindow, &researchWindow);
        winMgr->addContainer(hud.get());
        winMgr->addWindow(&optionsWindow);
        winMgr->addWindow(&researchWindow);
        winMgr->addWindow(&buildingSelectionWindow);
        optionsWindow.setGameState(gameState);
        optionsWindow.connect("stateChanged", [&](std::shared_ptr<world::GameState> state)
                              {
                                  auto starMapScene = std::make_shared<scenes::WorldScene>(renderer, sceneManager, state);
                                  core::SceneManager::Instance().changeScene("world", starMapScene);
                              });
    }
    WorldScene::~WorldScene()
    {

        thread->stop();
    }
    std::shared_ptr<world::Building> WorldScene::createBuilding(world::BuildingType type)
    {
        std::shared_ptr<world::Building> building = nullptr;
        graphics::Rect rect;
        rect.width = static_cast<float>(mapRenderer->getTileWidth());
        rect.height = static_cast<float>(mapRenderer->getTileHeight());
        int nextIndex = gameState->getPlayer()->getMaxBuildingIndex() + 1;
        std::string index = std::to_string(nextIndex);
        switch (type)
        {

        case world::BuildingType::Street:
            building = std::make_shared<world::buildings::Street>();
            rect.x = 0;
            rect.y = 128;
            building->setOffset(0, 0);
            building->setSourceRect(rect);
            break;
        default:
            building = services::BuildingService::Instance().create(type);
        }
        return building;
    }

    std::shared_ptr<world::Building> WorldScene::findBuilding(world::BuildingType type)
    {
        std::shared_ptr<world::Building> building = nullptr;
        graphics::Rect rect;
        rect.width = static_cast<float>(mapRenderer->getTileWidth());
        rect.height = static_cast<float>(mapRenderer->getTileHeight());

        switch (type)
        {

        case world::BuildingType::Street:
            building = std::make_shared<world::buildings::Street>();
            rect.x = 0;
            rect.y = 128;
            building->setOffset(0, 0);
            building->setSourceRect(rect);
            break;
        default:
            building = services::BuildingService::Instance().find(type).at(0);
        }
        return building;
    }

    void WorldScene::renderHUD()
    {

        int height = 40;
        buildWindow.setPos(0, height + 50);

        renderer->setDrawColor(0x00, 0xbc, 0xff, 128);
        renderer->setDrawBlendMode(SDL_BLENDMODE_BLEND);
        graphics::Rect hudRect = {0, 0, renderer->getViewPort().width, float(height)};
        renderer->fillRect(hudRect);

        renderer->setDrawColor(0xff, 0xff, 0xff, 128);
        utils::Vector2 start(0, height);
        utils::Vector2 end(renderer->getViewPort().width, height);
        renderer->drawLine(start, end);
        renderer->setDrawBlendMode(SDL_BLENDMODE_NONE);

        //render minimap
        auto &miniMap = mapRenderer->getMiniMap();
        const int miniMapSize = 150;
        miniMap->renderResized(renderer, renderer->getViewPort().width - miniMapSize, height, miniMapSize, miniMapSize);

        buildWindow.render(renderer);
    }

    void WorldScene::render()
    {
        auto &gameMap = gameState->getGameMap();
        auto &cities = gameState->getCities();

        mapRenderer->render(renderer);
        for (auto city : cities)
        {
            city->renderCity(renderer);
        }

        float factor = ceilf(renderer->getZoomFactor() * 100) / 100;

        auto camera = renderer->getMainCamera();
        for (float y = 0; y < cursorBuildingRect.height; y++)
        {
            for (float x = 0; x < cursorBuildingRect.width; x++)
            {

                float tx = float(cursorPosition.getX() + x) * mapRenderer->getTileWidth() / 2.0f;
                float ty = float(cursorPosition.getY() + y) * mapRenderer->getTileHeight();
                utils::Vector2 vec(tx, ty);
                const auto &pos = iso::twoDToIso(vec);

                float tileYOffset = mapRenderer->getTileYOffset(gameMap->getTile(cursorPosition.getX(), cursorPosition.getY()), cursorPosition.getX(), cursorPosition.getY());

                graphics::Rect srcRect;
                srcRect.x = 0;
                srcRect.y = 0;
                srcRect.width = cursorTexture->getWidth();
                srcRect.height = cursorTexture->getHeight();
                graphics::Rect destRect;
                destRect.x = (pos.getX() * factor) - camera->getX();
                destRect.y = ((pos.getY() + (mapRenderer->getTileHeight() - srcRect.height) - tileYOffset) * factor) - camera->getY();
                destRect.width = srcRect.width * factor;
                destRect.height = srcRect.height * factor;
                cursorTexture->render(renderer, srcRect, destRect);
            }
        }

        renderHUD();

        winMgr->render(renderer);

        if (optionsWindow.getVisible() && previewSurface == nullptr)
        {
            auto &win = core::GameWindow::Instance();
            previewSurface = SDL_CreateRGBSurface(0, win.getWidth(), win.getHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
            SDL_RenderReadPixels(renderer->getRenderer(), NULL, SDL_PIXELFORMAT_ARGB8888, previewSurface->pixels, previewSurface->pitch);

            optionsWindow.setPreview(previewSurface);
        }
    }

    void WorldScene::handleEvents(core::Input *pInput)
    {
        auto &gameMap = gameState->getGameMap();
        bool mouseIntersectsWindow = buildWindow.displayRect().intersects(pInput->getMousePostion());

        float height = 50;
        //

        graphics::Rect hudRect = {0, 0, renderer->getViewPort().width, height};

        if (!mouseIntersectsWindow && !winMgr->isWindowOpen() && !hudRect.intersects(pInput->getMousePostion()))
        {
            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                std::cout << "build here" << std::endl;
                auto action = buildWindow.getCurrentAction();
                if (action == world::BuildAction::Destroy)
                {
                    //check if building exists and then destroy it
                    const graphics::Rect &sourceBuilding = {cursorPosition.getX(), cursorPosition.getY(), 1, 1};

                    auto building = gameMap->getBuilding2D(sourceBuilding);
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
                        mapRenderer->clearCache();
                    }
                }
                else if (action == world::BuildAction::Build)
                {
                    auto building = createBuilding(buildWindow.getCurrentBuildingType());
                    building->setPosition(cursorPosition.getX(), cursorPosition.getY());

                    if (building != nullptr && building->canBuild(gameState->getPlayer()->getCash()) && gameMap->canBuild(building->get2DPosition()))
                    {
                        if (building->hasComponent("SalesComponent"))
                        {
                            auto sales = building->getComponent<world::buildings::SalesComponent>("SalesComponent");
                            sales->setGameMap(gameMap.get());
                        }
                        gameMap->addBuilding(building);
                        gameState->getPlayer()->addBuilding(building);
                        gameState->getPlayer()->incCash(building->getBuildPrice() * -1);
                        mapRenderer->clearCache();
                    }
                }
                else
                {
                    const graphics::Rect &sourceBuilding = {cursorPosition.getX(), cursorPosition.getY(), 1, 1};
                    auto building = gameMap->getBuilding2D(sourceBuilding);
                    std::shared_ptr<world::Company> company = nullptr;
                    if (gameState->getPlayer()->hasBuilding(building))
                    {
                        company = gameState->getPlayer();
                    }
                    auto rect = buildingWindow.displayRect();
                    int width = core::GameWindow::Instance().getWidth();
                    int height = core::GameWindow::Instance().getHeight();
                    buildingWindow.setPos(width / 2 - (rect.width / 2), height / 2 - (rect.height / 2));

                    buildingWindow.open(building, company, gameMap->getTile(cursorPosition), gameMap.get());
                }
            }
            else if (pInput->isMouseButtonPressed(SDL_BUTTON_RIGHT))
            {
                buildWindow.setCurrentAction(world::BuildAction::None);
            }
            if (pInput->isScrollWheel())
            {
                auto wheelPosition = pInput->getMouseWheelPosition();
                float offset = (wheelPosition.getY() / 10.f);
                float factor = renderer->getZoomFactor() + offset;
                if (factor >= 0.5f)
                {
                    renderer->setZoomFactor(factor);
                    float camX = renderer->getMainCamera()->getX();
                    float camY = renderer->getMainCamera()->getY();
                    renderer->getMainCamera()->reset();
                    renderer->getMainCamera()->move(camX * (1.f + offset), camY * (1.f + offset));
                    mapRenderer->clearCache();
                }

                std::cout << "factor: " << factor << std::endl;
            }
            if (pInput->isMouseMoving())
            {
                float camX = renderer->getMainCamera()->getX();
                float camY = renderer->getMainCamera()->getY();

                utils::Vector2 pt = iso::isoTo2D(pInput->getMousePostion() + utils::Vector2(camX, camY));

                float x, y = 0.0;

                float tx = pt.getX() / float(mapRenderer->getTileHeight() * renderer->getZoomFactor());
                float ty = pt.getY() / float(mapRenderer->getTileHeight() * renderer->getZoomFactor());

                x = std::round(tx - 0.5f);
                y = std::round(ty - 0.5f);

                cursorPosition = utils::Vector2(x, y);
                //std::cout << "mouse position x: " << x << " y:" << y << std::endl;

                auto building = (buildWindow.getCurrentAction() == world::BuildAction::Build) ? findBuilding(buildWindow.getCurrentBuildingType()) : nullptr;
                if (building != nullptr)
                    building->setPosition(cursorPosition.getX(), cursorPosition.getY());

                cursorTexture->setBlendMode(SDL_BLENDMODE_BLEND);
                cursorTexture->setAlphaMod(150);
                if (building != nullptr && building->canBuild(gameState->getPlayer()->getCash()) && gameMap->canBuild(building->get2DPosition()))
                {
                    cursorBuildingRect = building->get2DPosition();
                    cursorTexture->setColorKey(0, 255, 0);
                }
                else if (building != nullptr)
                {
                    cursorBuildingRect = building->get2DPosition();
                    cursorTexture->setColorKey(255, 0, 0);
                }
                else
                {
                    cursorBuildingRect = {cursorPosition.getX(), cursorPosition.getY(), 1, 1};
                    cursorTexture->setColorKey(255, 255, 255);
                }
            }

            if (pInput->isKeyDown(SDLK_DOWN) || pInput->isKeyDown(SDLK_s))
            {
                direction.bottom = true;
                direction.top = false;
            }
            else if (pInput->isKeyDown(SDLK_UP) || pInput->isKeyDown(SDLK_w))
            {
                direction.top = true;
                direction.bottom = false;
            }
            else
            {
                direction.top = false;
                direction.bottom = false;
            }

            if (pInput->isKeyDown(SDLK_LEFT) || pInput->isKeyDown(SDLK_a))
            {
                direction.left = true;
                direction.right = false;
            }
            else if (pInput->isKeyDown(SDLK_RIGHT) || pInput->isKeyDown(SDLK_d))
            {
                direction.left = false;
                direction.right = true;
            }
            else
            {
                direction.left = false;
                direction.right = false;
            }

            if (utils::areSame(pInput->getMousePostion().getX(), 0.f))
            {
                direction.left = true;
                direction.right = false;
            }
            else if (renderer->getMainCamera()->getWidth() - pInput->getMousePostion().getX() <= 5)
            {
                direction.left = false;
                direction.right = true;
            }

            if (utils::areSame(pInput->getMousePostion().getY(), 0.f))
            {
                direction.top = true;
                direction.bottom = false;
            }
            else if (renderer->getMainCamera()->getHeight() - pInput->getMousePostion().getY() <= 5)
            {
                direction.top = false;
                direction.bottom = true;
            }
        }

        if (pInput->isKeyDown(SDLK_ESCAPE))
        {
            //create preview image
            if (previewSurface != nullptr)
            {
                SDL_FreeSurface(previewSurface);
                previewSurface = nullptr;
            }
            optionsWindow.setPos(renderer->getViewPort().width / 2, renderer->getViewPort().height / 2);
            optionsWindow.setVisible(true);
        }
        else if (pInput->isKeyDown(SDLK_r))
        {
            auto rect = researchWindow.displayRect();
            researchWindow.setPos(renderer->getViewPort().width / 2 - (rect.width / 2), renderer->getViewPort().height / 2 - (rect.height / 2));
            researchWindow.setVisible(true);
        }

        buildWindow.handleEvents(pInput);
        winMgr->handleInput(pInput);
    }

    void WorldScene::update()
    {
        float speed = renderer->getTimeDelta() / 1000.f * 400.f;

        float moveX = 0.f;
        float moveY = 0.f;

        if (direction.top)
        {
            //if (viewPort.y > 0)
            moveY -= speed;
        }
        else if (direction.bottom)
        {
            moveY += speed;
        }

        if (direction.left)
        {
            //if (viewPort.x > 0)
            moveX -= speed;
        }
        else if (direction.right)
        {
            moveX += speed;
        }
        if (moveX != 0.0f || moveY != 0.0f)
        {
            renderer->getMainCamera()->move(moveX, moveY);
            moveX = 0;
            moveY = 0;
            wasMoving = true;

            if (renderer->getMainCamera()->getY() < 0)
            {
                moveY = renderer->getMainCamera()->getY() * -1;
            }
            renderer->getMainCamera()->move(moveX, moveY);
        }
    }

    void WorldScene::fixedUpdate(uint32_t delta)
    {

        hud->update();

        if (wasMoving)
        {
            mapRenderer->clearCache();
            wasMoving = false;
        }
    }
}
