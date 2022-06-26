#include "loan.h"
#include <cmath>

namespace world
{
    Loan::Loan(double amount, double interestRate, utils::time::Date startDay, utils::time::Date endOfContract)
        : m_amount(amount), m_interestRate(interestRate), m_startDay(startDay), m_endOfContract(endOfContract)
    {
        generateInstallments();
    }

    Loan::~Loan()
    {
    }

    std::vector<Installment> Loan::getInstallments()
    {
        return m_installments;
    }

    size_t Loan::calculateNumberOfInstallments()
    {
        size_t installments = 12 - m_startDay.getMonth();

        if (m_startDay.getDay() > 1)
        {
            installments--;
        }
        int currentYear = m_startDay.getYear() + 1;
        while (currentYear < m_endOfContract.getYear())
        {
            installments += 12;
            currentYear++;
        }
        installments += m_endOfContract.getMonth();
        return installments;
    }

    void Loan::generateInstallments()
    {
        size_t numberOfInstallments = calculateNumberOfInstallments();
        utils::time::Date installmentDate = m_startDay;
        if (m_startDay.getDay() > 1)
        {
            size_t lastDayOfMonth = utils::time::lastDayOfMonth(m_startDay.getYear(), m_startDay.getMonth());
            installmentDate.addDay(lastDayOfMonth - m_startDay.getDay());
        }
        double repaymentAmount = m_amount;
        for (size_t i = 0; i < numberOfInstallments; ++i)
        {
            size_t lastDayOfMonth = utils::time::lastDayOfMonth(installmentDate.getYear(), installmentDate.getMonth());
            installmentDate.addDay(lastDayOfMonth);
            double installmentAmount = std::round(m_amount / double(numberOfInstallments) * 100.0) / 100.0;
            repaymentAmount -= installmentAmount;

            double interest = std::round(repaymentAmount * (m_interestRate / 100.0) / 12.0 * 100.0) / 100.0;
            Installment installment(installmentDate, installmentAmount, interest);

            m_installments.push_back(installment);
        }
    }
    double Loan::calculateRepaymentWithInterest()
    {
        double value = 0.0;
        for (int idx = m_currentInstallment; i < m_installments.size(); ++idx)
        {
            value += m_installments[idx].installmentAmount;
            value += m_installments[idx].interest;
        }
        return value;
    }

    bool Loan::repayInstallment(Installment &installment)
    {

        installment = m_installments[m_currentInstallment];
        m_currentInstallment++;
        return m_currentInstallment < m_installments.size();
    }

}