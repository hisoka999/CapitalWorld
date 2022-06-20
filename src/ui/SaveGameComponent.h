#ifndef __SAVEGAMECOMPONENT_H__
#define __SAVEGAMECOMPONENT_H__

#include <engine/ui/Container.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/Button.h>
#include <engine/ui/Label.h>
#include <engine/ui/TextItem.h>
#include <filesystem>

namespace UI
{
    class SaveGameComponent : public UI::Container, public UI::Object
    {
    private:
        std::shared_ptr<UI::Label> fileDateLabel;
        std::shared_ptr<UI::TextItem> nameTextItem;
        std::shared_ptr<UI::ImageButton> previewImageButton;

        void initUI();

    public:
        SaveGameComponent(UI::Object *parent, bool loadGame, const std::filesystem::directory_entry &entry);
        ~SaveGameComponent();
        virtual void render(core::Renderer *pRender);
        virtual bool handleEvents(core::Input *pInput);

        std::shared_ptr<UI::Button> actionButton;
        std::shared_ptr<UI::Button> deleteButton;
    };

} // namespace UI

#endif // __SAVEGAMECOMPONENT_H__