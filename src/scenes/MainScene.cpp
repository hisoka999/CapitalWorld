/*
 * MainScene.cpp
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#include "MainScene.h"
#include <engine/ui/Button.h>
#include <functional>
#include <iostream>
#include <vector>
//#include "../translate.h"
#include <engine/utils/os.h>

namespace scenes
{

        MainScene::MainScene(core::Renderer *pRenderer,
                             core::SceneManager *pSceneManager) : core::Scene(pRenderer), running(true), sceneManager(pSceneManager)
        {
                uiTexture.loadTexture(renderer, "images/ArkanaLook.png");

                bgTexture.loadTexture(renderer, "images/title_background.png");

                container = new UI::Container();
                SDL_Color white =
                    {255, 255, 255, 0};
                auto btnStart = std::make_shared<UI::Button>();
                btnStart->setFont("fonts/Audiowide-Regular.ttf", 14);
                btnStart->setColor(white);
                btnStart->setLabel("New Game");
                btnStart->setPos(450, 350);
                btnStart->setStaticWidth(150);
                container->addObject(btnStart);

                btnStart->connect(UI::Button::buttonClickCallback(), [&]() { startGame(); });

                auto btnLoadGame = std::make_shared<UI::Button>();
                btnLoadGame->setFont("fonts/Audiowide-Regular.ttf", 14);
                btnLoadGame->setColor(white);
                btnLoadGame->setLabel("Load Game");
                btnLoadGame->setPos(450, 400);
                btnLoadGame->setStaticWidth(150);
                btnLoadGame->disable();

                btnLoadGame->connect(UI::Button::buttonClickCallback(), [&]() { loadGame(); });

                container->addObject(btnLoadGame);

                auto btnExit = std::make_shared<UI::Button>();
                btnExit->setFont("fonts/Audiowide-Regular.ttf", 14);
                btnExit->setColor(white);
                btnExit->setLabel("Exit Game");
                btnExit->setPos(450, 450);
                btnExit->setStaticWidth(150);

                btnExit->connect(UI::Button::buttonClickCallback(), [&]() { exitGame(); });

                container->addObject(btnExit);
        }
        void MainScene::render()
        {
                bgTexture.renderResized(renderer, 0, 0,
                                        renderer->getMainCamera()->getWidth(),
                                        renderer->getMainCamera()->getHeight());

                container->render(renderer);
        }

        void MainScene::exitGame()
        {
                running = false;
        }

        void MainScene::startGame()
        {
                std::cout << "start game" << std::endl;

                sceneManager->setCurrentScene("world");
        }

        void MainScene::loadGame()
        {
        }

        void MainScene::handleEvents(core::Input *pInput)
        {
                container->handleEvents(pInput);
        }

        MainScene::~MainScene()
        {
                delete container;
        }

} /* namespace scenes */
