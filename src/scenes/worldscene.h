#ifndef WORLDSCENE_H
#define WORLDSCENE_H

#include "ui/BuildingSelectionWindow.h"
#include "ui/buildingwindow.h"
#include "ui/buildwindow.h"
#include "ui/hudcontainer.h"
#include "ui/optionswindow.h"
#include "ui/researchwindow.h"
#include "world/buildactions.h"
#include "world/building.h"
#include "world/city.h"
#include "world/company.h"
#include "world/gamemaprenderer.h"
#include "world/gamestate.h"
#include "world/updatethread.h"
#include <engine/core/Scene.h>
#include <engine/core/SceneManager.h>
#include <map>
#include <world/gamemap.h>
#include "ui/Console.h"

namespace scenes
{

    struct MovementDirection
    {
        bool left = false;
        bool right = false;
        bool top = false;
        bool bottom = false;

        bool isMoving()
        {
            return left || right || top || bottom;
        }
    };

    class WorldScene : public core::Scene
    {
    public:
        WorldScene(core::Renderer *pRenderer,
                   core::SceneManager *pSceneManager, std::shared_ptr<world::GameState> gameState);
        ~WorldScene();
        void render();
        void handleEvents(core::Input *pInput);
        virtual void update();
        virtual void fixedUpdate(uint32_t delta);

    private:
        void renderHUD();
        std::shared_ptr<world::Building> createBuilding();

        graphics::Texture uiTexture;
        core::SceneManager *sceneManager;

        std::shared_ptr<GameMapRenderer> mapRenderer;
        std::shared_ptr<graphics::Texture> cursorTexture;
        std::shared_ptr<graphics::Texture> hudTexture;
        std::shared_ptr<graphics::Text> hudFont;
        utils::Vector2 cursorPosition;
        graphics::Rect cursorBuildingRect;
        UI::BuildWindow buildWindow;
        UI::ResearchWindow researchWindow;
        std::shared_ptr<world::GameState> gameState;
        UI::OptionsWindow optionsWindow;
        std::unique_ptr<UpdateThread> thread;
        UI::BuildingWindow buildingWindow;
        UI::BuildingSelectionWindow buildingSelectionWindow;
        UI::Console console;
        std::shared_ptr<UI::HUDContainer> hud;
        MovementDirection direction;
        bool wasMoving = false;
        double updateDelta = 0;
        SDL_Surface *previewSurface = nullptr;
        std::shared_ptr<world::Building> selectedBuilding2Build;
        };
}
#endif // WORLDSCENE_H
