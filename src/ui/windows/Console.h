#pragma once

#include <engine/ui/Window.h>
#include <engine/ui/TextItem.h>
#include <engine/ui/scrollarea.h>
#include <world/gamestate.h>

namespace UI
{
    class Console : public UI::Window
    {

    public:
        Console(const std::shared_ptr<world::GameState> &gameState);
        ~Console();

    private:
        void initUI();
        std::vector<std::string> executeCommand(std::string value);
        std::shared_ptr<UI::TextItem> textItem;
        std::shared_ptr<UI::ScrollArea> scrollArea;
        std::shared_ptr<world::GameState> gameState;
    };

} // namespace ui
