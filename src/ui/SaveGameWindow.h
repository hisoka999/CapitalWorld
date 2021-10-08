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
        std::shared_ptr<UI::Button> saveButton;
        bool load;
        SDL_Surface *previewSurface;

    public:
        SaveGameWindow(bool load);
        void updateSaveGames();
        ~SaveGameWindow();
        void setPreview(SDL_Surface *previewSurface);
    };

} // namespace UI

#endif // __SAVEGAMEWINDOW_H__