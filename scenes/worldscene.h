#ifndef WORLDSCENE_H
#define WORLDSCENE_H

#include <engine/core/Scene.h>
#include <engine/core/SceneManager.h>
#include <world/gamemap.h>
#include <world/gamemaprenderer.h>
#include <ui/buildwindow.h>
#include <world/company.h>
#include <world/buildactions.h>
#include <world/building.h>
#include <world/gamestate.h>
#include <world/updatethread.h>
#include <ui/buildingwindow.h>
#include <map>

namespace scenes {


class WorldScene :public core::Scene
{
public:
    WorldScene(core::Renderer *pRenderer,
               core::SceneManager* pSceneManager);
    ~WorldScene();
    void render();
    void handleEvents(core::Input *pInput);


private:
    void renderHUD();
    std::shared_ptr<world::Building> createBuilding(world::BuildAction action);
    graphics::Texture uiTexture;
    core::SceneManager *sceneManager;
    std::shared_ptr<GameMap> gameMap;
    std::shared_ptr<GameMapRenderer> mapRenderer;
    std::shared_ptr<graphics::Texture> cursorTexture;
    std::shared_ptr<graphics::Texture> hudTexture;
    std::shared_ptr<graphics::Text> hudFont;
    utils::Vector2 cursorPosition;
    UI::BuildWindow buildWindow;
    std::shared_ptr<world::GameState> gameState;
    std::unique_ptr<UpdateThread> thread;
    UI::BuildingWindow buildingWindow;

};
}
#endif // WORLDSCENE_H
