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
#include <ui/windows/SettingsWindow.h>
#include <ui/windows/SaveGameWindow.h>

namespace scenes
{

        class MainScene : public core::Scene
        {
        public:
                MainScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager, std::shared_ptr<utils::IniBase> settings, core::Input *input);
                virtual ~MainScene();
                void render();
                bool handleEvents(core::Input *pInput);
                bool isRunning()
                {
                        return running;
                }

                virtual void load();

        private:
                bool running;
                core::SceneManager *sceneManager;
                UI::Container container;
                graphics::Texture bgTexture;
                SettingsWindow settingsWindow;
                UI::SaveGameWindow loadWindow;

                void exitGame();
                void startGame();
                void loadGame();
        };

} /* namespace scenes */

#endif /* SCENES_MAINSCENE_H_ */
