#ifndef __OPTIONSWINDOW_H__
#define __OPTIONSWINDOW_H__

#include <engine/ui/Window.h>
#include <engine/ui/scrollarea.h>
#include "ui/SettingsWindow.h"

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

    private:
        std::shared_ptr<UI::ScrollArea> mainArea;
        SettingsWindow settingsWindow;
    };

} // namespace UI

#endif // __OPTIONSWINDOW_H__