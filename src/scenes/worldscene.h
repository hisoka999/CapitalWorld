#ifndef WORLDSCENE_H
#define WORLDSCENE_H

#include "ui/windows/BuildingSelectionWindow.h"
#include "ui/windows/Console.h"
#include "ui/windows/buildingwindow.h"
#include "ui/windows/buildwindow.h"
#include "ui/hudcontainer.h"
#include "ui/windows/optionswindow.h"
#include "ui/windows/researchwindow.h"
#include "ui/windows/PlayerWindow.h"
#include "world/AIThread.h"
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
#include "notifications/EventQueue.h"
#include "world/AnimatedMovement.h"
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
                   core::SceneManager *pSceneManager, std::shared_ptr<world::GameState> gameState, std::shared_ptr<utils::IniBase> settings, core::Input *input);
        ~WorldScene();
        virtual void unload() override;
        void render();
        bool handleEvents(core::Input *pInput);
        virtual void update();
        virtual void fixedUpdate(uint32_t delta);
        std::shared_ptr<world::GameState> &getGameState();

    private:
        void refresh();
        void renderHUD();
        void renderCursor();
        bool handleMouseEvents(core::Input *pInput);
        void calcCursorPosition(core::Input *pInput);
        std::shared_ptr<world::Building> createBuilding();

        core::SceneManager *sceneManager;
        UI::BuildingSelectionWindow buildingSelectionWindow;
        UI::BuildWindow buildWindow;
        UI::BuildingWindow buildingWindow;
        std::shared_ptr<world::GameState> gameState;
        UI::OptionsWindow optionsWindow;
        UI::ResearchWindow researchWindow;
        UI::Console console;
        UI::PlayerWindow playerWindow;

        graphics::Texture uiTexture;

        std::shared_ptr<GameMapRenderer> mapRenderer;
        std::shared_ptr<graphics::Texture> cursorTexture;
        std::shared_ptr<graphics::Text> hudFont;
        utils::Vector2 cursorPosition;
        graphics::Rect cursorBuildingRect;

        std::unique_ptr<UpdateThread> thread;
        std::unique_ptr<world::AIThread> aiThread;
        std::shared_ptr<UI::HUDContainer> hud;
        MovementDirection direction;
        bool wasMoving = false;
        double updateDelta = 0;
        SDL_Surface *previewSurface = nullptr;
        std::shared_ptr<world::Building> selectedBuilding2Build;
        int buildMessageRefId;
        int eventQueueRefId;
        int animationMessageRefId;
        graphics::Rect miniMapRect;
        bool dragBuildActive = false;
        notifications::EventQueue eventQueue;
        std::vector<std::string> playList;
        int currentFile = -1;
    };
}
#endif // WORLDSCENE_H
