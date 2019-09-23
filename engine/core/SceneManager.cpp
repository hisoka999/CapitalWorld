/*
 * SceneManager.cpp
 *
 *  Created on: 26.04.2016
 *      Author: stefan
 */

#include "engine/core/SceneManager.h"

namespace core
{
SceneManager* SceneManager::instance = nullptr;
std::once_flag SceneManager::onceFlag;
SceneManager::SceneManager()
{
    // TODO Auto-generated constructor stub

}

SceneManager::~SceneManager()
{
    // TODO Auto-generated destructor stub
}

void SceneManager::render()
{
    if (currentScene.empty())
        return;
    scenes[currentScene]->render();
}
void SceneManager::handleEvents(core::Input* pInput)
{
    if (currentScene.empty())
        return;
    scenes[currentScene]->handleEvents(pInput);

}

void SceneManager::update() {
    if (currentScene.empty())
        return;
    scenes[currentScene]->update();

}

void SceneManager::addScene(std::string name, core::Scene* scene)
{
    scenes.insert(std::pair<std::string, Scene*>(name, scene));
}

void SceneManager::setCurrentScene(std::string name)
{
    if (scenes.count(name) == 0) {
        throw std::runtime_error(
                "scene with name \"" + name + "\" does not exists.");
    }
    currentScene = name;
}

Scene* SceneManager::getScene(std::string name)
{
    return scenes[name];
}
Scene* SceneManager::getCurrentScene() {
    return scenes[currentScene];
}
} /* namespace core */


