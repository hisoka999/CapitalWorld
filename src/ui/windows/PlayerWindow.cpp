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

        auto bankTab = std::make_shared<UI::Tab>(m_tabBar.get(), _("Bank"));
        m_tabBar->addTab(bankTab);

        auto createLoanButton = std::make_shared<UI::Button>(bankTab.get());
        createLoanButton->setPos(10, 10);
        bankTab->addObject(createLoanButton);
        createLoanButton->setFont("fonts/arial.ttf", 12);
        createLoanButton->setLabel("Add Loan");

        createLoanButton->connect(UI::Button::buttonClickCallback(), [&]()
                                  {
            double amount = m_gameState->getPlayer()->calculateCompanyValue()*0.7;

            if(amount > 0){
                utils::time::Date contractEnd(m_gameState->getTime().getYear()+3,1,1);
                world::Loan loan(amount,5,m_gameState->getTime(),contractEnd);

                m_gameState->getPlayer()->addLoan(loan);
            } });

        m_balanceTable = std::make_shared<UI::Table<world::ProductBalance>>(profitTab.get());
        m_balanceTable->setPos(5, 5);

        std::vector<std::string> accountTableNames = {_("Account"), _("Costs"), _("Income")};
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
        std::vector<std::string> names = {_("Company"), _("Income"), _("Profit"), _("Cash")};
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

        refresh();
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
        for (auto data : player->displayData())
        {
            auto labelName = std::make_shared<UI::Label>(data.first, m_playerTab.get());
            auto labelValue = std::make_shared<UI::Label>(data.second, m_playerTab.get());
            optionalItems.push_back(labelName);
            optionalItems.push_back(labelValue);
            m_playerTab->addObject(labelName);
            m_playerTab->addObject(labelValue);
        }
        graphics::Rect bounds = m_tabBar->displayRect();
        bounds.x = 5;
        bounds.y = 5;
        m_playerTabLayout->updateLayout(bounds);
    }
}