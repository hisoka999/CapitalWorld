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
#include <engine/utils/vector2.h>
#include <future>
#include <iostream>
#include <random>
#include "messages.h"

namespace scenes
{

    WorldScene::WorldScene(core::Renderer *pRenderer,
                           core::SceneManager *pSceneManager, std::shared_ptr<world::GameState> gameState)

        : core::Scene(pRenderer), sceneManager(
                                      pSceneManager),
          buildingSelectionWindow(200, 100, gameState->getPlayer()), buildWindow(0, static_cast<int>(pRenderer->getViewPort().height / 2.0f), &buildingSelectionWindow), buildingWindow(100, 100), gameState(gameState), optionsWindow(0, 0), researchWindow(gameState), console(gameState), playerWindow(gameState)
    {
        cursorTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "cursor.png"));
        hudTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "ui_base.png"));
        hudFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "arial.ttf"), 16);

        mapRenderer = std::make_shared<GameMapRenderer>(gameState);

        renderer->setZoomFactor(1);

        console.setFont(hudFont.get());
        console.setVisible(false);

        thread = std::make_unique<UpdateThread>(gameState);
        aiThread = std::make_unique<world::AIThread>(gameState);
        winMgr->addWindow(&buildingWindow);
        winMgr->addWindow(&console);

        hud = std::make_shared<UI::HUDContainer>(thread.get(), aiThread.get(), gameState, &optionsWindow, &researchWindow, &playerWindow);
        winMgr->addContainer(hud.get());
        winMgr->addWindow(&optionsWindow);
        winMgr->addWindow(&researchWindow);
        winMgr->addWindow(&buildingSelectionWindow);
        winMgr->addWindow(&playerWindow);
        buildingSelectionWindow.connect("buildingSelectionChanged", [&](std::shared_ptr<world::Building> building)
                                        { selectedBuilding2Build = building; });
        optionsWindow.setGameState(gameState);
        optionsWindow.connect("stateChanged", [&](std::shared_ptr<world::GameState> state)
                              {
                                  auto worldScene = std::make_shared<scenes::WorldScene>(renderer, sceneManager, state);
                                  core::SceneManager::Instance().changeScene("world", worldScene); });

        core::MessageSystem<MessageTypes>::get().registerForType(MessageTypes::ObjectHasBuild, [&]([[maybe_unused]] bool dummy)
                                                                 { 
                                                                     if(mapRenderer != nullptr)
                                                                     mapRenderer->clearCache(); });
    }
    WorldScene::~WorldScene()
    {

        thread->stop();
        aiThread->stop();
        if (previewSurface != nullptr)
        {
            SDL_FreeSurface(previewSurface);
        }
    }

    void WorldScene::renderHUD()
    {

        float height = 40;
        const int miniMapSize = 150;

        buildWindow.setPos(0, height);
        buildWindow.setSize(200, renderer->getViewPort().height - 40);
        buildingSelectionWindow.setSize(300, renderer->getViewPort().height - 40);
        buildingSelectionWindow.setPos(200, height);

        renderer->setDrawColor(0x22, 0x22, 0x22, 255);
        renderer->setDrawBlendMode(SDL_BLENDMODE_BLEND);
        graphics::Rect hudRect = {0, 0, renderer->getViewPort().width, height};
        renderer->fillRect(hudRect);
        graphics::Rect miniMapRect = {renderer->getViewPort().width - miniMapSize, height, float(miniMapSize), float(miniMapSize)};

        renderer->fillRect(miniMapRect);

        renderer->setDrawColor(0xff, 0xff, 0xff, 128);
        utils::Vector2 start(0, height);
        utils::Vector2 end(renderer->getViewPort().width - miniMapSize, height);
        renderer->drawLine(start, end);
        auto newEnd = end + utils::Vector2(0, miniMapSize);
        auto lastEnd = newEnd + utils::Vector2(miniMapSize, 0);
        renderer->drawLine(end, newEnd);
        renderer->drawLine(newEnd, lastEnd);

        renderer->setDrawBlendMode(SDL_BLENDMODE_NONE);

        // render minimap
        auto &miniMap = mapRenderer->getMiniMap();
        miniMap->renderResized(renderer, miniMapRect.x, miniMapRect.y, miniMapRect.width, miniMapRect.height);

        buildWindow.render(renderer);
        buildWindow.postRender(renderer);
    }

    std::shared_ptr<world::Building> WorldScene::createBuilding()
    {
        std::shared_ptr<world::Building> building = nullptr;
        graphics::Rect rect;
        rect.width = static_cast<float>(mapRenderer->getTileWidth());
        rect.height = static_cast<float>(mapRenderer->getTileHeight());
        int nextIndex = gameState->getPlayer()->getMaxBuildingIndex() + 1;
        std::string index = std::to_string(nextIndex);
        switch (selectedBuilding2Build->getType())
        {

        case world::BuildingType::Street:
            building = std::make_shared<world::buildings::Street>();
            rect.x = 0;
            rect.y = 128;
            building->setOffset(0, 0);
            building->setSourceRect(rect);
            break;
        default:
            building = services::BuildingService::Instance().create(selectedBuilding2Build);
        }
        return building;
    }

    void WorldScene::render()
    {

        mapRenderer->render(renderer);

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

                float tileYOffset = 0.0;

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
                SDL_Color red = {255, 0, 0, 255};

                std::string cursorPositionText = utils::string_format("%i / %i", int(cursorPosition.getX()), int(cursorPosition.getY()));
                hudFont->render(renderer, cursorPositionText, red, 0, 700);
            }
        }
        auto &win = core::GameWindow::Instance();

        renderHUD();

        winMgr->render(renderer);

        if (optionsWindow.getVisible() && previewSurface == nullptr)
        {
            previewSurface = SDL_CreateRGBSurface(0, win.getWidth(), win.getHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
            SDL_RenderReadPixels(renderer->getRenderer(), NULL, SDL_PIXELFORMAT_ARGB8888, previewSurface->pixels, previewSurface->pitch);

            optionsWindow.setPreview(previewSurface);
        }
    }

    bool WorldScene::handleEvents(core::Input *pInput)
    {
        bool eventHandled = false;
        auto &gameMap = gameState->getGameMap();
        // bool mouseIntersectsWindow = buildWindow.displayRect().intersects(pInput->getMousePostion());

        float height = 50;
        //

        eventHandled = winMgr->handleInput(pInput);
        if (eventHandled)
            return true;

        eventHandled = buildWindow.handleEvents(pInput);
        if (eventHandled)
            return true;

        graphics::Rect hudRect = {0, 0, renderer->getViewPort().width, height};

        if (!hudRect.intersects(pInput->getMousePostion()))
        {
            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                std::cout << "build here" << std::endl;
                auto action = buildWindow.getCurrentAction();
                if (action == world::BuildAction::Destroy)
                {
                    // check if building exists and then destroy it
                    const graphics::Rect &sourceBuilding = {cursorPosition.getX(), cursorPosition.getY(), 1, 1};

                    auto building = gameMap->getBuilding2D(sourceBuilding);
                    if (building != nullptr)
                    {
                        // remove it

                        // get back cash
                        if (gameState->getPlayer()->hasBuilding(building))
                        {
                            gameMap->removeBuilding(building);
                            gameState->getPlayer()->removeBuilding(building);
                            gameState->getPlayer()->incCash(building->getBuildPrice() * 0.2f);
                        }
                        else if (gameState->findBuildingOwner(building) == nullptr)
                        {
                            gameMap->removeBuilding(building);
                            // if no one owns the building you have to pay for destroying it
                            gameState->getPlayer()->incCash(building->getBuildPrice() * -1.0f);
                        }
                        mapRenderer->clearCache();
                        eventHandled = true;
                    }
                }
                else if (action == world::BuildAction::Build)
                {
                    if (!selectedBuilding2Build)
                        return eventHandled;
                    auto building = createBuilding();
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
                        eventHandled = true;
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

                    buildingWindow.open(building, gameState, cursorPosition, gameMap.get());
                    eventHandled = true;
                }
            }
            else if (pInput->isMouseButtonPressed(SDL_BUTTON_RIGHT))
            {
                buildWindow.setCurrentAction(world::BuildAction::None);
                buildingSelectionWindow.setSelectedBuilding(nullptr);
                eventHandled = true;
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
                eventHandled = true;
            }
            if (pInput->isMouseMoving())
            {
                float camX = renderer->getMainCamera()->getX();
                float camY = renderer->getMainCamera()->getY();

                float factor = ceilf(renderer->getZoomFactor() * 100) / 100;

                float mouse_x = pInput->getMousePostion().getX() + camX;
                float mouse_y = pInput->getMousePostion().getY() + camY;
                float tile_height = float(mapRenderer->getTileHeight()) * factor;
                float tile_width = float(mapRenderer->getTileWidth()) * factor;

                mouse_x -= tile_width / 2;
                mouse_y -= tile_height / 2;

                float mouse_grid_x = floor((mouse_y / tile_height) + (mouse_x / tile_width));
                float mouse_grid_y = floor((-mouse_x / tile_width) + (mouse_y / tile_height));
                cursorPosition = utils::Vector2(mouse_grid_x, mouse_grid_y);

                auto building = selectedBuilding2Build;
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
                eventHandled = true;
            }

            if (pInput->isKeyDown(SDLK_DOWN) || pInput->isKeyDown(SDLK_s))
            {
                direction.bottom = true;
                direction.top = false;
                eventHandled = true;
            }
            else if (pInput->isKeyDown(SDLK_UP) || pInput->isKeyDown(SDLK_w))
            {
                direction.top = true;
                direction.bottom = false;
                eventHandled = true;
            }
            else
            {
                direction.top = false;
                direction.bottom = false;
                eventHandled = true;
            }

            if (pInput->isKeyDown(SDLK_LEFT) || pInput->isKeyDown(SDLK_a))
            {
                direction.left = true;
                direction.right = false;
                eventHandled = true;
            }
            else if (pInput->isKeyDown(SDLK_RIGHT) || pInput->isKeyDown(SDLK_d))
            {
                direction.left = false;
                direction.right = true;
                eventHandled = true;
            }
            else
            {
                direction.left = false;
                direction.right = false;
                eventHandled = true;
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

            if (pInput->isKeyDown(SDLK_PAGEDOWN))
            {
                console.setVisible(true);
                eventHandled = true;
            }
        }

        if (pInput->isKeyDown(SDLK_ESCAPE))
        {
            // create preview image
            if (previewSurface != nullptr)
            {
                SDL_FreeSurface(previewSurface);
                previewSurface = nullptr;
            }
            optionsWindow.setPos(renderer->getViewPort().width / 2, renderer->getViewPort().height / 2);
            optionsWindow.setVisible(true);
            eventHandled = true;
        }
        else if (pInput->isKeyDown(SDLK_r) && !SDL_IsTextInputActive())
        {
            auto rect = researchWindow.displayRect();
            researchWindow.setPos(renderer->getViewPort().width / 2 - (rect.width / 2), renderer->getViewPort().height / 2 - (rect.height / 2));
            researchWindow.setVisible(true);
            eventHandled = true;
        }
        else if (pInput->isKeyDown(SDLK_l))
        {
            mapRenderer->toggleDebug();
        }

        return eventHandled;
    }

    void WorldScene::update()
    {
        core::MessageSystem<MessageTypes>::get().processMessages();
        float speed = renderer->getTimeDelta() / 1000.f * 400.f;

        float moveX = 0.f;
        float moveY = 0.f;

        if (direction.top)
        {
            // if (viewPort.y > 0)
            moveY -= speed;
        }
        else if (direction.bottom)
        {
            moveY += speed;
        }

        if (direction.left)
        {
            // if (viewPort.x > 0)
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
        updateDelta += renderer->getTimeDelta();
        if (wasMoving && updateDelta > 50)
        {
            mapRenderer->clearCache();
            wasMoving = false;
            updateDelta = 0;
        }
    }

    void WorldScene::fixedUpdate([[maybe_unused]] uint32_t delta)
    {

        hud->update();
    }

    std::shared_ptr<world::GameState> &WorldScene::getGameState()
    {
        return gameState;
    }
}
