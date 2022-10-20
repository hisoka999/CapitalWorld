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
#include "world/buildings/TransportComponent.h"

namespace scenes
{

    WorldScene::WorldScene(core::Renderer *pRenderer,
                           core::SceneManager *pSceneManager, std::shared_ptr<world::GameState> gameState, std::shared_ptr<utils::IniBase> settings)

        : core::Scene(pRenderer), sceneManager(
                                      pSceneManager),
          buildingSelectionWindow(200, 100, gameState->getPlayer()), buildWindow(0, static_cast<int>(pRenderer->getViewPort().height / 2.0f), &buildingSelectionWindow), buildingWindow(100, 100), gameState(gameState), optionsWindow(0, 0, settings), researchWindow(gameState), console(gameState), playerWindow(gameState)
    {
        cursorTexture = graphics::TextureManager::Instance().loadTexture(utils::os::combine("images", "cursor.png"));
        hudFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "arial.ttf"), 16);

        mapRenderer = std::make_shared<GameMapRenderer>(gameState);
        cursorBuildingRect = {0.f, 0.f, 1.f, 1.f};

        renderer->setZoomFactor(1);

        console.setFont(hudFont.get());
        console.setVisible(false);

        renderer->getMainCamera()->move(renderer->getMainCamera()->getWidth() / 2.0f * -1.f, 0.0f);

        thread = std::make_unique<UpdateThread>(gameState);
        aiThread = std::make_unique<world::AIThread>(gameState);
        winMgr->addWindow(&buildingWindow);
        winMgr->addWindow(&console);

        hud = std::make_shared<UI::HUDContainer>(renderer, thread.get(), aiThread.get(), gameState, &optionsWindow, &researchWindow, &playerWindow);
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
                                  auto worldScene = std::make_shared<scenes::WorldScene>(renderer, sceneManager, state,settings);
                                  core::SceneManager::Instance().changeScene("world", worldScene); });

        buildMessageRefId = core::MessageSystem<MessageTypes>::get().registerForType(MessageTypes::ObjectHasBuild, [this]([[maybe_unused]] bool dummy)
                                                                                     { refresh(); });

        eventQueueRefId = core::MessageSystem<MessageTypes>::get().registerForType(MessageTypes::Event, [this](notifications::Event event)
                                                                                   { eventQueue.add(event); });

        animationMessageRefId = core::MessageSystem<MessageTypes>::get().registerForType(MessageTypes::AnimationStart, [this](world::AnimatedMovementData movement)
                                                                                         { 
                                                            auto texture = ::graphics::TextureManager::Instance().loadTexture("images/sprites/"+movement.colorName+"_vehicles.png");
                                                            auto sprite = std::make_shared<world::graphics::Sprite>(texture, world::graphics::SpriteDirection::LeftDown, 8, 0);
                                                            std::unique_ptr<world::AnimatedMovement>  animation = std::make_unique<world::AnimatedMovement>(movement.route->path, sprite);

                                                            animation->setFinishCallback([=](){
                                                                movement.route->transportFinished = true;
                                                            });

                                                            mapRenderer->addAnimation(std::move(animation)); });
    }
    WorldScene::~WorldScene()
    {
        core::MessageSystem<MessageTypes>::get().deregister(buildMessageRefId);
        core::MessageSystem<MessageTypes>::get().deregister(eventQueueRefId);
        core::MessageSystem<MessageTypes>::get().deregister(animationMessageRefId);
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
        miniMapRect = {renderer->getViewPort().width - miniMapSize, height, float(miniMapSize), float(miniMapSize)};

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

        // render camera position
        auto cameraRect = renderer->getMainCamera()->getViewPortRect();
        // map site
        int mapWidth = mapRenderer->getTileWidth() * gameState->getGameMap()->getWidth();
        int mapHeight = mapRenderer->getTileHeight() * gameState->getGameMap()->getHeight();

        float relativeX = miniMapRect.width / renderer->getZoomFactor() / float(mapWidth);
        float relativeY = miniMapRect.height / renderer->getZoomFactor() / float(mapHeight);
        cameraRect.x = miniMapRect.x + (miniMapRect.width / 2.0f) + (relativeX * cameraRect.x);
        cameraRect.y = miniMapRect.y + (relativeY * cameraRect.y);
        cameraRect.width *= relativeX;
        cameraRect.height *= relativeY;

        renderer->drawRect(cameraRect);

        buildWindow.render(renderer);
        buildWindow.postRender(renderer);

        eventQueue.render(renderer);
    }

    void WorldScene::renderCursor()
    {
        float factor = ceilf(renderer->getZoomFactor() * 100) / 100;

        auto camera = renderer->getMainCamera();
        graphics::Rect elementRect;

        for (float y = 0; y != cursorBuildingRect.height; y += std::copysign(1.0, cursorBuildingRect.height))
        {
            for (float x = 0; x != cursorBuildingRect.width; x += std::copysign(1.0, cursorBuildingRect.width))
            {

                float tx = float(cursorBuildingRect.x + x) * mapRenderer->getTileWidth() / 2.0f;
                float ty = float(cursorBuildingRect.y + y) * mapRenderer->getTileHeight();
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

                elementRect = {cursorBuildingRect.x + x, cursorBuildingRect.y + y, 1, 1};

                if (selectedBuilding2Build != nullptr && selectedBuilding2Build->canBuild(gameState->getPlayer()->getCash()) && gameState->getGameMap()->canBuild(elementRect))
                {
                    cursorTexture->setColorKey(0, 255, 0);
                }
                else if (selectedBuilding2Build != nullptr)
                {

                    cursorTexture->setColorKey(255, 0, 0);
                }
                else
                {

                    cursorTexture->setColorKey(255, 255, 255);
                }

                cursorTexture->render(renderer, srcRect, destRect);
                SDL_Color red = {255, 0, 0, 255};

                std::string cursorPositionText = utils::string_format("%i / %i", int(cursorPosition.getX()), int(cursorPosition.getY()));
                hudFont->render(renderer, cursorPositionText, red, 0, 700);
            }
        }
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

        renderCursor();
        renderHUD();

        winMgr->render(renderer);

        if (optionsWindow.getVisible() && previewSurface == nullptr)
        {
            previewSurface = SDL_CreateRGBSurface(0, renderer->getMainCamera()->getWidth(), renderer->getMainCamera()->getHeight(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
            SDL_RenderReadPixels(renderer->getRenderer(), NULL, SDL_PIXELFORMAT_ARGB8888, previewSurface->pixels, previewSurface->pitch);

            optionsWindow.setPreview(previewSurface);
        }
    }

    bool WorldScene::handleMouseEvents(core::Input *pInput)
    {
        auto &gameMap = gameState->getGameMap();

        bool eventHandled = false;
        if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            auto action = buildWindow.getCurrentAction();

            if (action == world::BuildAction::Build && selectedBuilding2Build->canDragBuild())
            {
                calcCursorPosition(pInput);
                if (!dragBuildActive)
                {
                    cursorBuildingRect.x = cursorPosition.getX();
                    cursorBuildingRect.y = cursorPosition.getY();
                    cursorBuildingRect.width = 1;
                    cursorBuildingRect.height = 1;
                    dragBuildActive = true;
                    return true;
                }
                else
                {
                }
                eventHandled = true;
            }
            else if (action == world::BuildAction::None)
            {
                const graphics::Rect &sourceBuilding = {cursorPosition.getX(), cursorPosition.getY(), 1, 1};
                auto building = gameMap->getBuilding2D(sourceBuilding);
                std::shared_ptr<world::Company> company = nullptr;
                if (gameState->getPlayer()->hasBuilding(building))
                {
                    company = gameState->getPlayer();
                }
                auto rect = buildingWindow.displayRect();
                int width = renderer->getMainCamera()->getWidth();
                int height = renderer->getMainCamera()->getHeight();
                buildingWindow.setPos(width / 2.f - (rect.width / 2.f), height / 2.f - (rect.height / 2.f));

                buildingWindow.open(building, gameState, cursorPosition, gameMap.get());
                eventHandled = true;
            }
        }
        else if (pInput->isMouseButtonUp(SDL_BUTTON_LEFT))
        {
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

                if (selectedBuilding2Build->canDragBuild())
                {
                    for (float y = 0; y != cursorBuildingRect.height; y += std::copysign(1.0, cursorBuildingRect.height))
                    {
                        for (float x = 0; x != cursorBuildingRect.width; x += std::copysign(1.0, cursorBuildingRect.width))
                        {
                            auto building = createBuilding();
                            building->setPosition(cursorBuildingRect.x + x, cursorBuildingRect.y + y);

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
                            }
                        }
                    }
                    mapRenderer->clearCache();
                    eventHandled = true;
                    dragBuildActive = false;
                }
                else
                {
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
                        dragBuildActive = false;
                    }
                }
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

            APP_LOG_TRACE("factor: " + std::to_string(factor));
            eventHandled = true;
        }
        if (pInput->isMouseMoving())
        {
            calcCursorPosition(pInput);

            auto building = selectedBuilding2Build;
            if (building != nullptr)
                building->setPosition(cursorPosition.getX(), cursorPosition.getY());

            cursorTexture->setBlendMode(SDL_BLENDMODE_BLEND);
            cursorTexture->setAlphaMod(150);

            if (building != nullptr && building->canDragBuild() && dragBuildActive)
            {

                cursorBuildingRect.width = cursorPosition.getX() - cursorBuildingRect.x;
                if (cursorBuildingRect.width == 0.0f)
                    cursorBuildingRect.width = 1.0f;
                cursorBuildingRect.height = cursorPosition.getY() - cursorBuildingRect.y;
                if (cursorBuildingRect.height == 0.0f)
                    cursorBuildingRect.height = 1.0f;
            }

            else if (building != nullptr)
                cursorBuildingRect = building->get2DPosition();
            else
                cursorBuildingRect = {cursorPosition.getX(), cursorPosition.getY(), 1, 1};

            eventHandled = true;
        }
        return eventHandled;
    }

    void WorldScene::calcCursorPosition(core::Input *pInput)
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
    }

    bool WorldScene::handleEvents(core::Input *pInput)
    {
        bool eventHandled = false;

        //

        eventHandled = winMgr->handleInput(pInput);
        if (eventHandled)
            return true;

        eventHandled = buildWindow.handleEvents(pInput);
        if (eventHandled)
            return true;

        if (miniMapRect.intersects(pInput->getMousePostion()) && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            // render camera position
            // map size
            int mapWidth = mapRenderer->getTileWidth() * gameState->getGameMap()->getWidth();
            int mapHeight = mapRenderer->getTileHeight() * gameState->getGameMap()->getHeight();

            // reletive size incl. zoom
            float relativeX = miniMapRect.width / renderer->getZoomFactor() / float(mapWidth);
            float relativeY = miniMapRect.height / renderer->getZoomFactor() / float(mapHeight);

            // new position  = -(half of camera size) + (mouse position  - minimap position) / relative size - (half of map size)
            int newX = (renderer->getMainCamera()->getWidth() / 2.0f * -1.0f) + ((pInput->getMousePostion().getX() - miniMapRect.x) / relativeX) - (mapWidth / 2.0f * renderer->getZoomFactor());
            int newY = (renderer->getMainCamera()->getHeight() / 2.0f * -1.0f) + (pInput->getMousePostion().getY() - miniMapRect.y) / relativeY;
            renderer->getMainCamera()->reset();
            renderer->getMainCamera()->move(newX, newY);
            mapRenderer->clearCache();
            return true;
        }

        eventHandled = handleMouseEvents(pInput);

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
        else if (pInput->isKeyDown(SDLK_r) && !pInput->isTextInputActive())
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
            moveY -= speed;
        }
        else if (direction.bottom)
        {
            moveY += speed;
        }

        if (direction.left)
        {
            moveX -= speed;
        }
        else if (direction.right)
        {
            moveX += speed;
        }
        if (moveX != 0.0f || moveY != 0.0f)
        {
            int mapWidth = mapRenderer->getTileWidth() * gameState->getGameMap()->getWidth();
            int mapHeight = mapRenderer->getTileHeight() * gameState->getGameMap()->getHeight();
            float factor = renderer->getZoomFactor();

            const auto &cameraRect = renderer->getMainCamera()->getViewPortRect();
            int targetX = cameraRect.x + moveX;
            int targetY = cameraRect.y + moveY;
            if (targetX < (mapWidth / 2 * -1 * factor))
            {
                moveX = 0;
            }
            else if (targetX + (cameraRect.width) > (mapWidth / 2 * factor))
            {
                moveX = 0;
            }

            if (targetY + (cameraRect.height) > (mapHeight * factor))
            {
                moveY = 0;
            }
            else if (targetY < 0)
            {
                moveY = 0;
            }
            renderer->getMainCamera()->move(moveX, moveY);
            moveX = 0;
            moveY = 0;
            wasMoving = true;

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
        eventQueue.updateEvents(delta);
        if (!thread->getPaused())
            mapRenderer->update(delta, thread->getSpeed());
    }

    std::shared_ptr<world::GameState> &WorldScene::getGameState()
    {
        return gameState;
    }

    void WorldScene::refresh()
    {
        if (mapRenderer != nullptr)
            mapRenderer->clearCache();
    }
}