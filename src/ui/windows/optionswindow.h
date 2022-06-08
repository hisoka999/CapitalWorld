#ifndef __OPTIONSWINDOW_H__
#define __OPTIONSWINDOW_H__

#include "SaveGameWindow.h"
#include "ui/windows/SettingsWindow.h"
#include "world/gamestate.h"
#include <engine/ui/Window.h>
#include <engine/ui/scrollarea.h>
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
        virtual bool handleEvents(core::Input *pInput);
        void setGameState(const std::shared_ptr<world::GameState> &gameState);
        const std::shared_ptr<world::GameState> &getGameState();
        void setPreview(SDL_Surface *previewSurface);

    private:
        void saveGame();
        void loadGame();
        std::shared_ptr<UI::ScrollArea> mainArea;
        SettingsWindow settingsWindow;
        std::shared_ptr<world::GameState> gameState;
        UI::SaveGameWindow saveWindow;
        UI::SaveGameWindow loadWindow;
        SDL_Surface *previewSurface;
    };

} // namespace UI

#endif // __OPTIONSWINDOW_H__