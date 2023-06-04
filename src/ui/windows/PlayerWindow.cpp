#include "PlayerWindow.h"
#include <engine/utils/string.h>
#include <magic_enum.hpp>
#include <messages.h>
#include "translate.h"
#include <engine/ui/layout/GridLayout.h>
#include <engine/ui/Label.h>
#include <unordered_map>

namespace UI
{
    PlayerWindow::PlayerWindow(const std::shared_ptr<world::GameState> &gameState) : UI::Window(0, 0, 700, 400), m_gameState(gameState)
    {
        setTitle("Player");

        m_tabBar = std::make_shared<UI::TabBar>(this);
        m_tabBar->setWidth(580);
        m_tabBar->setHeight(400);
        addObject(m_tabBar);
        m_playerTab = std::make_shared<UI::Tab>(m_tabBar.get(), _("Company"));
        m_tabBar->addTab(m_playerTab);
        m_playerTabLayout = std::make_shared<UI::layout::GridLayout>(m_playerTab.get(), 2);

        auto profitTab = std::make_shared<UI::Tab>(m_tabBar.get(), _("Profit/Loss"));
        m_tabBar->addTab(profitTab);

        m_loanTab = std::make_shared<UI::LoanTab>(m_gameState, m_tabBar.get());
        m_tabBar->addTab(m_loanTab);

        m_balanceTable = std::make_shared<UI::Table<world::ProductBalance>>(profitTab.get());
        m_balanceTable->setPos(5, 5);

        std::vector<std::string> accountTableNames = {_("Account"), _("Costs"), _("Income")};
        m_balanceTable->setHeaderNames(accountTableNames);
        m_balanceTable->setElementFunction(0, [](std::shared_ptr<world::ProductBalance> &c) -> std::string
                                           { return std::string(magic_enum::enum_name<world::BalanceAccount>(c->account)); });

        m_balanceTable->setElementFunction(1, [](std::shared_ptr<world::ProductBalance> &c) -> std::string
                                           { return format_currency(c->costs); });
        m_balanceTable->setElementFunction(2, [](std::shared_ptr<world::ProductBalance> &c) -> std::string
                                           { return format_currency(c->income); });
        m_balanceTable->setWidth(480);
        profitTab->addObject(m_balanceTable);
        m_balanceTable->setHeight(340);

        auto topCompaniesTab = std::make_shared<UI::Tab>(m_tabBar.get(), _("Top Companies"));
        m_tabBar->addTab(topCompaniesTab);

        m_playerTable = std::make_shared<UI::Table<world::Company>>(topCompaniesTab.get());
        m_playerTable->setPos(5, 5);
        auto companies = gameState->getCompanies();
        m_playerTable->setData(companies);
        std::vector<std::string> names = {_("Company"), _("Income"), _("Profit"), _("Cash")};
        m_playerTable->setHeaderNames(names);
        m_playerTable->setElementFunction(0, [](std::shared_ptr<world::Company> &c) -> std::string
                                          { return c->getName(); });

        m_playerTable->setElementFunction(1, [](std::shared_ptr<world::Company> &c) -> std::string
                                          { return format_currency(c->getIncome()); });
        m_playerTable->setElementFunction(2, [](std::shared_ptr<world::Company> &c) -> std::string
                                          { return format_currency(c->getMonthlyProfit()); });
        m_playerTable->setElementFunction(3, [](std::shared_ptr<world::Company> &c) -> std::string
                                          { return format_currency(c->getCash()); });
        m_playerTable->setWidth(480);
        m_playerTable->setHeight(340);
        topCompaniesTab->addObject(m_playerTable);

        auto &msgSystem = core::MessageSystem<MessageTypes>::get();

        m_messageId = msgSystem.registerForType(MessageTypes::NewMonth, [=]()
                                                { needsRefresh(); });

        refresh();
    }

    PlayerWindow::~PlayerWindow()
    {
        core::MessageSystem<MessageTypes>::get().deregister(m_messageId);
    }

    void PlayerWindow::refresh()
    {
        int year = m_gameState->getTime().getYear();
        auto balance = m_gameState->getPlayer()->getAccountBalanceForYear(year);
        m_balanceTable->setData(balance);
        auto &player = m_gameState->getPlayer();

        for (auto &item : optionalItems)
        {
            m_playerTab->removeObject(item);
        }
        optionalItems.clear();
        for (auto &data : player->displayData())
        {
            auto labelName = std::make_shared<UI::Label>(data.key, m_playerTab.get());
            auto labelValue = std::make_shared<UI::Label>(data.value, m_playerTab.get());
            optionalItems.push_back(labelName);
            optionalItems.push_back(labelValue);
            m_playerTab->addObject(labelName);
            m_playerTab->addObject(labelValue);
        }
        graphics::Rect bounds = m_tabBar->displayRect();
        bounds.x = 5;
        bounds.y = 5;
        m_playerTabLayout->updateLayout(bounds);

        m_loanTab->updateData();
    }
}