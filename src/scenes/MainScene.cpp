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
#include "translate.h"
#include <engine/utils/os.h>
#include <engine/core/gamewindow.h>
#include <engine/utils/os.h>
#include <engine/utils/json/parser.h>
#include <fstream>
#include "world/gamestate.h"
#include "scenes/worldscene.h"
namespace scenes
{

        MainScene::MainScene(core::Renderer *pRenderer,
                             core::SceneManager *pSceneManager, std::shared_ptr<utils::IniBase> settings)
            : core::Scene(pRenderer), running(true), sceneManager(pSceneManager), settingsWindow(settings), loadWindow(true)
        {

                bgTexture.loadTexture(renderer, "images/title_background.png");

                this->music->loadMusic("music/Juhani Junkala [Retro Game Music Pack] Title Screen.wav");
                int volume = settings->getValueI("Volume", "Music");
                this->music->setVolume(volume);
                winMgr->addContainer(&container);

                SDL_Color white =
                    {255, 255, 255, 0};
                auto btnStart = std::make_shared<UI::Button>();
                btnStart->setFont("fonts/arial.ttf", 14);
                btnStart->setColor(white);
                btnStart->setLabel(_("New Game"));
                btnStart->setPos(450, 350);
                btnStart->setStaticWidth(150);
                container.addObject(btnStart);

                btnStart->connect(UI::Button::buttonClickCallback(), [&]()
                                  { startGame(); });

                auto btnLoadGame = std::make_shared<UI::Button>();
                btnLoadGame->setFont("fonts/arial.ttf", 14);
                btnLoadGame->setColor(white);
                btnLoadGame->setLabel(_("Load Game"));
                btnLoadGame->setPos(450, 400);
                btnLoadGame->setStaticWidth(150);
                // btnLoadGame->disable();

                btnLoadGame->connect(UI::Button::buttonClickCallback(), [&]()
                                     { loadGame(); });

                container.addObject(btnLoadGame);

                auto btnSettings = std::make_shared<UI::Button>();
                btnSettings->setFont("fonts/arial.ttf", 14);
                btnSettings->setColor(white);
                btnSettings->setLabel(_("Settings"));
                btnSettings->setPos(450, 450);
                btnSettings->setStaticWidth(150);

                btnSettings->connect(UI::Button::buttonClickCallback(), [&]()
                                     { settingsWindow.setVisible(true); });

                container.addObject(btnSettings);

                auto btnExit = std::make_shared<UI::Button>();
                btnExit->setFont("fonts/arial.ttf", 14);
                btnExit->setColor(white);
                btnExit->setLabel(_("Exit Game"));
                btnExit->setPos(450, 500);
                btnExit->setStaticWidth(150);

                btnExit->connect(UI::Button::buttonClickCallback(), [&]()
                                 { exitGame(); });

                container.addObject(btnExit);
                winMgr->addWindow(&settingsWindow);
                winMgr->addWindow(&loadWindow);

                loadWindow.connect("saveAction", [&](std::string fileName)
                                   {
                               std::string saveGameFile = utils::os::get_pref_dir("", "capitalworld") + "/saves/" + fileName + ".save";
                               std::ifstream file;
                               std::istringstream is;
                               std::string s;
                               std::string group;

                               file.open(saveGameFile.c_str(), std::ios::in);
                               if (!file.is_open())
                               {
                                   throw IOException(saveGameFile, "file does not exists");
                               }
                               std::string buffer((std::istreambuf_iterator<char>(file)),
                                                  std::istreambuf_iterator<char>());
                               utils::JSON::Parser parser;
                               auto jsonObject = parser.parseObject(buffer);

                               file.close();

                               auto gameState = world::GameState::fromJson(jsonObject);
                               auto worldScene = std::make_shared<scenes::WorldScene>(renderer, sceneManager, gameState,m_gameWindow->getSettings());
                               sceneManager->addScene("world", worldScene);
                               sceneManager->setCurrentScene("world"); });
        }
        void MainScene::render()
        {
                bgTexture.renderResized(renderer, 0, 0,
                                        renderer->getMainCamera()->getWidth(),
                                        renderer->getMainCamera()->getHeight());

                winMgr->render(renderer);
        }

        void MainScene::exitGame()
        {
                running = false;
        }

        void MainScene::startGame()
        {
                APP_LOG_INFO("start game");

                sceneManager->setCurrentScene("newGameScene");
        }

        void MainScene::loadGame()
        {
                loadWindow.updateSaveGames();
                loadWindow.setVisible(true);
        }

        bool MainScene::handleEvents(core::Input *pInput)
        {
                return winMgr->handleInput(pInput);
        }

        void MainScene::load()
        {
                music->play(-1);
        }

        MainScene::~MainScene()
        {
        }

} /* namespace scenes */
