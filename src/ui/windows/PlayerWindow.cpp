#include "PlayerWindow.h"
#include <engine/utils/string.h>
#include <magic_enum.hpp>
#include <messages.h>

namespace UI
{
    PlayerWindow::PlayerWindow(const std::shared_ptr<world::GameState> &gameState) : UI::Window(0, 0, 700, 400), m_gameState(gameState)
    {
        setTitle("Player");

        m_tabBar = std::make_shared<UI::TabBar>(this);
        m_tabBar->setWidth(580);
        m_tabBar->setHeight(400);
        addObject(m_tabBar);
        auto playerTab = std::make_shared<UI::Tab>(m_tabBar.get(), "Company");
        m_tabBar->addTab(playerTab);
        auto profitTab = std::make_shared<UI::Tab>(m_tabBar.get(), "Profit/Loss");
        m_tabBar->addTab(profitTab);

        m_balanceTable = std::make_shared<UI::Table<world::ProductBalance>>(profitTab.get());
        m_balanceTable->setPos(5, 5);

        std::vector<std::string> accountTableNames = {"Account", "Costs", "Income"};
        m_balanceTable->setHeaderNames(accountTableNames);
        m_balanceTable->setElementFunction(0, [](std::shared_ptr<world::ProductBalance> &c) -> std::string
                                           { return std::string(magic_enum::enum_name<world::BalanceAccount>(c->account)); });

        m_balanceTable->setElementFunction(1, [](std::shared_ptr<world::ProductBalance> &c) -> std::string
                                           { return utils::string_format(u8"%.2f €", c->costs); });
        m_balanceTable->setElementFunction(2, [](std::shared_ptr<world::ProductBalance> &c) -> std::string
                                           { return utils::string_format(u8"%.2f €", c->income); });
        m_balanceTable->setWidth(480);
        profitTab->addObject(m_balanceTable);
        m_balanceTable->setHeight(340);

        auto topCompaniesTab = std::make_shared<UI::Tab>(m_tabBar.get(), "Top Companies");
        m_tabBar->addTab(topCompaniesTab);

        m_playerTable = std::make_shared<UI::Table<world::Company>>(topCompaniesTab.get());
        m_playerTable->setPos(5, 5);
        auto companies = gameState->getCompanies();
        m_playerTable->setData(companies);
        std::vector<std::string> names = {"Company", "Income", "Profit", "Cash"};
        m_playerTable->setHeaderNames(names);
        m_playerTable->setElementFunction(0, [](std::shared_ptr<world::Company> &c) -> std::string
                                          { return c->getName(); });

        m_playerTable->setElementFunction(1, [](std::shared_ptr<world::Company> &c) -> std::string
                                          { return utils::string_format(u8"%.2f €", c->getIncome()); });
        m_playerTable->setElementFunction(2, [](std::shared_ptr<world::Company> &c) -> std::string
                                          { return utils::string_format(u8"%.2f €", c->getProfit()); });
        m_playerTable->setElementFunction(3, [](std::shared_ptr<world::Company> &c) -> std::string
                                          { return utils::string_format(u8"%.2f €", c->getCash()); });
        m_playerTable->setWidth(480);
        m_playerTable->setHeight(340);
        topCompaniesTab->addObject(m_playerTable);

        auto &msgSystem = core::MessageSystem<MessageTypes>::get();

        msgSystem.registerForType(MessageTypes::NewMonth, [=]()
                                  { needsRefresh(); });
    }

    void PlayerWindow::refresh()
    {
        int year = m_gameState->getTime().getYear();
        auto balance = m_gameState->getPlayer()->getAccountBalanceForYear(year);
        m_balanceTable->setData(balance);
    }
}