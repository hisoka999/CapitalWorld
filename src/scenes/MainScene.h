/*
 * MainScene.h
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#ifndef SCENES_MAINSCENE_H_
#define SCENES_MAINSCENE_H_

#include <engine/core/Scene.h>
#include <engine/core/renderer.h>
#include <engine/core/SceneManager.h>
#include <engine/ui/Container.h>
#include <vector>
#include <memory>
#include <ui/SettingsWindow.h>
#include <ui/SaveGameWindow.h>

namespace scenes
{

        class MainScene : public core::Scene
        {
        public:
                MainScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager);
                virtual ~MainScene();
                void render();
                void handleEvents(core::Input *pInput);
                bool isRunning()
                {
                        return running;
                }

        private:
                bool running;
                core::SceneManager *sceneManager;
                UI::Container *container;
                graphics::Texture uiTexture;
                graphics::Texture bgTexture;
                SettingsWindow settingsWindow;
                UI::SaveGameWindow loadWindow;

                void exitGame();
                void startGame();
                void loadGame();
        };

} /* namespace scenes */

#endif /* SCENES_MAINSCENE_H_ */
