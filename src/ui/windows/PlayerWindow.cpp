#include "PlayerWindow.h"
#include <engine/utils/string.h>

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
    }
}