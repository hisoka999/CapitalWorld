#include "LoanTab.h"
#include <engine/ui/Button.h>
#include <engine/ui/Label.h>
#include <translate.h>
#include <engine/ui/layout/GridLayout.h>
namespace UI
{
    LoanTab::LoanTab(const std::shared_ptr<world::GameState> &gameState, UI::Object *parent)
        : UI::Tab(parent, _("Loans")), m_gameState(gameState)
    {
        m_loanDataContainer = std::make_shared<UI::ScrollArea>(480, 140, this);
        m_loanDataContainer->setPos(5, 5);

        addObject(m_loanDataContainer);
        auto layout = std::make_unique<UI::layout::GridLayout>(m_loanDataContainer.get(), 2);
        auto loneSizeLabel = std::make_shared<UI::Label>(m_loanDataContainer.get());
        loneSizeLabel->setFont("fonts/arial.ttf", 12);
        loneSizeLabel->setText(_("Loan size: "));
        m_loanDataContainer->addObject(loneSizeLabel);

        m_loneSizeCombobox = std::make_shared<UI::ComboBox<int>>(m_loanDataContainer.get());
        m_loneSizeCombobox->setWidth(150);
        m_loneSizeCombobox->setElementFunction([](int value)
                                               { return std::to_string(value); });

        int maxLoneSize = m_gameState->getPlayer()->calculateCompanyValue() * 0.7;
        if (maxLoneSize > 0)
        {
            m_loneSizeCombobox->addElement(maxLoneSize);
            m_loneSizeCombobox->addElement(maxLoneSize * 0.75);
            m_loneSizeCombobox->addElement(maxLoneSize * 0.5);
        }
        else
        {
            m_loneSizeCombobox->addElement(0);
        }
        m_loanDataContainer->addObject(m_loneSizeCombobox);

        auto interestRateLabel = std::make_shared<UI::Label>(m_loanDataContainer.get());
        interestRateLabel->setFont("fonts/arial.ttf", 12);
        interestRateLabel->setText(_("Interest rate: "));
        m_loanDataContainer->addObject(interestRateLabel);

        auto interestRateValueLabel = std::make_shared<UI::Label>(m_loanDataContainer.get());
        interestRateValueLabel->setFont("fonts/arial.ttf", 12);
        interestRateValueLabel->setTextF("%.2f %%", 5.0f);
        m_loanDataContainer->addObject(interestRateValueLabel);

        auto createLoanButton = std::make_shared<UI::Button>(m_loanDataContainer.get());
        createLoanButton->setPos(10, 10);
        m_loanDataContainer->addObject(createLoanButton);
        createLoanButton->setFont("fonts/arial.ttf", 12);
        createLoanButton->setLabel("Add Loan");

        createLoanButton->connect(UI::Button::buttonClickCallback(), [&]()
                                  {
            double amount = m_loneSizeCombobox->getSelectionText();
            if(amount > 0){
                utils::time::Date contractEnd(m_gameState->getTime().getYear()+3,1,1);
                auto  loan = std::make_shared<world::Loan>(amount,5,m_gameState->getTime(),contractEnd);

                m_gameState->getPlayer()->addLoan(loan);
            } });

        layout->setPadding(utils::Vector2(2, 2));
        graphics::Rect bounds = {5, 5, 480, 140};
        layout->updateLayout(bounds);

        m_loanTable = std::make_shared<UI::Table<world::Loan>>(this);
        addObject(m_loanTable);
        std::vector<std::string> headerNames = {"Amount", "Start Date", "End Date", "Repayment", "Interest"};
        m_loanTable->setHeaderNames(headerNames);
        m_loanTable->setElementFunction(0, [](std::shared_ptr<world::Loan> &c) -> std::string
                                        { return utils::string_format("%.2f €", c->getAmount()); });

        m_loanTable->setElementFunction(1, [](std::shared_ptr<world::Loan> &c) -> std::string
                                        { return c->getStartDay().format(); });
        m_loanTable->setElementFunction(2, [](std::shared_ptr<world::Loan> &c) -> std::string
                                        { return c->getEndOfContract().format(); });
        m_loanTable->setElementFunction(3, [](std::shared_ptr<world::Loan> &c) -> std::string
                                        { return utils::string_format(u8"%.2f €", c->calculateRepaymentWithInterest()); });
        m_loanTable->setElementFunction(4, [](std::shared_ptr<world::Loan> &c) -> std::string
                                        { return utils::string_format(u8"%.2f €", c->calculateRepaymentWithInterest()); });
        m_loanTable->setWidth(480);
        m_loanTable->setHeight(200);
        m_loanTable->setPos(5, 150);
    }

    LoanTab::~LoanTab()
    {
    }

    void LoanTab::updateData()
    {
        m_loneSizeCombobox->clearElements();
        int maxLoneSize = m_gameState->getPlayer()->calculateCompanyValue() * 0.7;
        if (maxLoneSize > 0)
        {
            m_loneSizeCombobox->addElement(maxLoneSize);
            m_loneSizeCombobox->addElement(maxLoneSize * 0.75);
            m_loneSizeCombobox->addElement(maxLoneSize * 0.5);
        }
        else
        {
            m_loneSizeCombobox->addElement(0);
        }
        m_loanTable->setData(m_gameState->getPlayer()->getActiveLoans());
    }
}