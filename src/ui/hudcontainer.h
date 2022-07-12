#pragma once
#include "ui/windows/optionswindow.h"
#include "ui/windows/PlayerWindow.h"
#include "ui/windows/researchwindow.h"
#include "world/updatethread.h"
#include <engine/ui/Container.h>
#include <world/AIThread.h>

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
        HUDContainer(UpdateThread *updateThread, world::AIThread *aiThread, const std::shared_ptr<world::GameState> &gameState, UI::OptionsWindow *optionsWindow, UI::ResearchWindow *researchWindow, UI::PlayerWindow *playerWindow);

        void update();

    private:
        void initUI();
        std::shared_ptr<graphics::Text> uiText;
        std::shared_ptr<graphics::Text> glyphText;
        std::shared_ptr<UI::Button> playButton;
        std::shared_ptr<UI::Button> pauseButton;
        std::shared_ptr<UI::Button> doubleSpeed;
        std::shared_ptr<UI::Button> fullSpeed;
        std::shared_ptr<UI::IconButton> companyButton;
        std::shared_ptr<UI::IconButton> cashButton;
        std::shared_ptr<UI::IconButton> profitButton;
        std::shared_ptr<UI::IconButton> optionsButton;
        std::shared_ptr<UI::IconButton> researchButton;
        std::shared_ptr<UI::IconButton> timeButton;

        UpdateThread *updateThread;
        world::AIThread *aiThread;
        std::shared_ptr<world::GameState> gameState;
        UI::OptionsWindow *optionsWindow;
        UI::ResearchWindow *researchWindow;
        UI::PlayerWindow *playerWindow;
    };
};