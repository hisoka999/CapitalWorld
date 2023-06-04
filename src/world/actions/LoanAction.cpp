#include "LoanAction.h"

namespace world
{
    namespace actions
    {
        LoanAction::LoanAction(const std::shared_ptr<world::Company> &company)
            : Action(company)
        {
        }

        LoanAction::~LoanAction()
        {
        }

        void LoanAction::execute(const std::shared_ptr<world::GameState> &gameState)
        {

            double amount = m_company->calculateCompanyValue() * 0.7;

            if (amount > 0)
            {
                utils::time::Date contractEnd(gameState->getTime().getYear() + 3, 1, 1);
                std::shared_ptr<world::Loan> loan = std::make_shared<world::Loan>(amount, 5, gameState->getTime(), contractEnd);

                m_company->addLoan(loan);
            }
        }

        bool LoanAction::canExecute([[maybe_unused]] const std::shared_ptr<world::GameState> &gameState)
        {
            return m_company->calculateCompanyValue() > 0 && m_company->getMonthlyProfit() > 0 && m_company->numberOfLoans() <= 10;
        }
    }
}