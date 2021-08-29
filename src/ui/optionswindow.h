#ifndef __OPTIONSWINDOW_H__
#define __OPTIONSWINDOW_H__

#include <engine/ui/Window.h>
#include <engine/ui/scrollarea.h>
#include "ui/SettingsWindow.h"
#include "world/gamestate.h"
#include "SaveGameWindow.h"
namespace UI
{
    class OptionsWindow : public UI::Window
    {
    private:
        /* data */
    public:
        OptionsWindow(int x, int y);
        ~OptionsWindow();

        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);
        void setGameState(const std::shared_ptr<world::GameState> &gameState);
        const std::shared_ptr<world::GameState> &getGameState();

    private:
        void saveGame();
        void loadGame();
        std::shared_ptr<UI::ScrollArea> mainArea;
        SettingsWindow settingsWindow;
        std::shared_ptr<world::GameState> gameState;
        UI::SaveGameWindow saveWindow;
        UI::SaveGameWindow loadWindow;
    };

} // namespace UI

#endif // __OPTIONSWINDOW_H__