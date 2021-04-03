/*
 * SceneManager.cpp
 *
 *  Created on: 26.04.2016
 *      Author: stefan
 */

#include "engine/core/SceneManager.h"
#include "engine/core/Scene.h"

namespace core
{

    SceneManager::SceneManager()
    {
        // TODO Auto-generated constructor stub
    }

    SceneManager::~SceneManager()
    {
        scenes.clear();
    }

    void SceneManager::render()
    {
        if (currentScene.empty())
            return;
        scenes[currentScene]->render();
    }
    void SceneManager::handleEvents(core::Input *pInput)
    {
        if (currentScene.empty())
            return;
        scenes[currentScene]->handleEvents(pInput);
    }

    void SceneManager::update()
    {
        if (currentScene.empty())
            return;
        scenes[currentScene]->update();
    }

    void SceneManager::addScene(std::string name, const std::shared_ptr<Scene> &scene)
    {

        scenes[name] = scene;
    }

    void SceneManager::setCurrentScene(std::string name)
    {
        if (currentScene.size() != 0)
        {
            auto &scene = scenes[currentScene];
            scene->unload();
        }
        if (scenes.count(name) == 0)
        {
            throw std::runtime_error(
                "scene with name \"" + name + "\" does not exists.");
        }
        currentScene = name;
    }

    const std::shared_ptr<Scene> &SceneManager::getScene(std::string name)
    {
        return scenes[name];
    }
    const std::shared_ptr<Scene> &SceneManager::getCurrentScene()
    {
        return scenes[currentScene];
    }
} /* namespace core */
