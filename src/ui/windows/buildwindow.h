#ifndef BUILDWINDOW_H
#define BUILDWINDOW_H

#include <engine/ui/Window.h>

#include "world/buildactions.h"
#include "ui/windows/BuildingSelectionWindow.h"
#include "world/buildingtypes.h"
#include <engine/graphics/texture.h>
#include <memory>

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
        void setCurrentAction(world::BuildAction action);
        std::shared_ptr<world::Building> getSelectedBuilding();

    private:
        void initUI();
        std::shared_ptr<graphics::Texture> backgroundTexture;
        world::BuildAction currentAction;
        world::BuildingType currentBuildingType;
        SDL_Cursor *cursor;
        UI::BuildingSelectionWindow *buildingSelectionWindow;
        std::shared_ptr<world::Building> selectedBuilding;
    };

}
#endif // BUILDWINDOW_H
