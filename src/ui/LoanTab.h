#pragma once

#include <engine/ui/Tab.h>
#include <world/gamestate.h>
#include <engine/ui/ComboBox.h>
#include <engine/ui/scrollarea.h>
#include <engine/ui/Table.h>
namespace UI
{
    class LoanTab : public UI::Tab
    {

    public:
        LoanTab(const std::shared_ptr<world::GameState> &gameState, UI::Object *parent);
        ~LoanTab();
        void updateData();

    private:
        std::shared_ptr<world::GameState> m_gameState;

        std::shared_ptr<UI::ComboBox<int>> m_loneSizeCombobox;
        std::shared_ptr<UI::ScrollArea> m_loanDataContainer;
        std::shared_ptr<UI::Table<world::Loan>> m_loanTable;
    };

} // namespace UI
