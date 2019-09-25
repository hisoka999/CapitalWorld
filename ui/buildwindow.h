#ifndef BUILDWINDOW_H
#define BUILDWINDOW_H

#include <engine/ui/Container.h>

#include <engine/graphics/texture.h>
#include <memory>
#include "world/buildactions.h"

namespace UI
{

class BuildWindow : public UI::Container,public UI::Object
{
public:
    BuildWindow(int x, int y);
    ~BuildWindow();
    virtual void render(core::Renderer *pRender, graphics::Texture *pTexture);
    virtual void handleEvents(core::Input *pInput);

    world::BuildAction getCurrentAction()
    {
        return currentAction;
    }
    void setCurrentAction(world::BuildAction action)
    {
        currentAction = action;
    }

private:
    void initUI();
    std::shared_ptr<graphics::Texture> backgroundTexture;
    world::BuildAction currentAction;


};

}
#endif // BUILDWINDOW_H
