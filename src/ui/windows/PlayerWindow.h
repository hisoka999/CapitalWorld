#pragma once
#include <engine/ui/Window.h>
#include <engine/ui/TabBar.h>
#include <engine/ui/Table.h>
#include <world/gamestate.h>
#include <engine/ui/layout/Layout.h>
namespace UI
{
#pragma once
    class PlayerWindow : public UI::Window
    {
    public:
        PlayerWindow(const std::shared_ptr<world::GameState> &gameState);

    protected:
        void refresh();

    private:
        std::shared_ptr<world::GameState> m_gameState;
        std::shared_ptr<UI::TabBar> m_tabBar;
        std::shared_ptr<UI::Tab> m_playerTab;
        std::shared_ptr<UI::layout::Layout> m_playerTabLayout;
        std::shared_ptr<UI::Table<world::Company>> m_playerTable;
        std::shared_ptr<UI::Table<world::ProductBalance>> m_balanceTable;
        std::vector<std::shared_ptr<UI::Object>> optionalItems;
    };
}
