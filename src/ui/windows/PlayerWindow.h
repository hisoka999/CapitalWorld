#pragma once
#include <engine/ui/Window.h>
#include <engine/ui/TabBar.h>
#include <engine/ui/Table.h>
#include <world/gamestate.h>

namespace UI
{
#pragma once
    class PlayerWindow : public UI::Window
    {
    public:
        PlayerWindow(const std::shared_ptr<world::GameState> &gameState);

    private:
        std::shared_ptr<world::GameState> m_gameState;
        std::shared_ptr<UI::TabBar> m_tabBar;
        std::shared_ptr<UI::Table<world::Company>> m_playerTable;
    };
}
