#ifndef BUILDWINDOW_H
#define BUILDWINDOW_H

#include <engine/ui/Window.h>

#include <engine/graphics/texture.h>
#include <memory>
#include "../world/buildactions.h"

namespace UI
{

    class BuildWindow : public UI::Window
    {
    public:
        BuildWindow(int x, int y);
        ~BuildWindow();
        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);

        world::BuildAction getCurrentAction()
        {
            return currentAction;
        }
        void setCurrentAction(world::BuildAction action);

    private:
        void initUI();
        std::shared_ptr<graphics::Texture> backgroundTexture;
        world::BuildAction currentAction;
        SDL_Cursor *cursor;
    };

}
#endif // BUILDWINDOW_H
