#ifndef BUILDWINDOW_H
#define BUILDWINDOW_H

#include <engine/ui/Window.h>

#include <engine/graphics/texture.h>
#include <memory>
#include "../world/buildactions.h"
#include "world/buildingtypes.h"
#include "ui/BuildingSelectionWindow.h"

namespace UI
{

    class BuildWindow : public UI::Window
    {
    public:
        BuildWindow(int x, int y, UI::BuildingSelectionWindow *buildingSelectionWindow);
        ~BuildWindow();
        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);

        world::BuildAction getCurrentAction()
        {
            return currentAction;
        }
        world::BuildingType getCurrentBuildingType();
        void setCurrentAction(world::BuildAction action);

    private:
        void initUI();
        std::shared_ptr<graphics::Texture> backgroundTexture;
        world::BuildAction currentAction;
        world::BuildingType currentBuildingType;
        SDL_Cursor *cursor;
        UI::BuildingSelectionWindow *buildingSelectionWindow;
    };

}
#endif // BUILDWINDOW_H
