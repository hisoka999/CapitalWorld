#ifndef __SAVEGAMEWINDOW_H__
#define __SAVEGAMEWINDOW_H__

#include <engine/ui/Window.h>
#include <engine/ui/scrollarea.h>
#include <vector>

namespace UI
{
    class SaveGameWindow : public UI::Window
    {
    private:
        std::shared_ptr<UI::ScrollArea> saveGameArea;
        bool load;

    public:
        SaveGameWindow(bool load);
        void updateSaveGames();
        ~SaveGameWindow();
    };

} // namespace UI

#endif // __SAVEGAMEWINDOW_H__