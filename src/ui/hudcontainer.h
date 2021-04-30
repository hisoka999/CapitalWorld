#pragma once
#include <engine/ui/Container.h>
#include "../world/updatethread.h"
#include "buildwindow.h"

namespace UI
{
    class IconButton;
    class Button;
}

namespace UI
{
    class HUDContainer : public UI::Container
    {
    public:
        HUDContainer(UpdateThread *updateThread, const std::shared_ptr<world::GameState> &gameState, UI::BuildWindow *buildWindow);

        virtual void render(core::Renderer *renderer);
        void update();

    private:
        void initUI();
        std::shared_ptr<graphics::Text> uiText;
        std::shared_ptr<graphics::Text> glyphText;
        std::shared_ptr<UI::Button> playButton;
        std::shared_ptr<UI::Button> pauseButton;
        std::shared_ptr<UI::Button> doubleSpeed;
        std::shared_ptr<UI::Button> fullSpeed;
        std::shared_ptr<UI::IconButton> cashButton;
        std::shared_ptr<UI::IconButton> profitButton;
        std::shared_ptr<UI::IconButton> buildButton;

        UpdateThread *updateThread;
        std::shared_ptr<world::GameState> gameState;
        UI::BuildWindow *buildWindow;
    };
};