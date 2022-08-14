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
        for (size_t idx = m_currentInstallment; idx < m_installments.size(); ++idx)
        {
            value += m_installments[idx].installmentAmount;
            value += m_installments[idx].interest;
        }
        return value;
    }

    double Loan::calculateRepayment()
    {
        double value = 0.0;
        for (size_t idx = m_currentInstallment; idx < m_installments.size(); ++idx)
        {
            value += m_installments[idx].installmentAmount;
        }
        return value;
    }
    double Loan::calculateInterest()
    {
        double value = 0.0;
        for (size_t idx = m_currentInstallment; idx < m_installments.size(); ++idx)
        {
            value += m_installments[idx].interest;
        }
        return value;
    }

    bool Loan::repayInstallment(Installment &installment)
    {

        installment = m_installments[m_currentInstallment];
        m_currentInstallment++;
        return m_currentInstallment == m_installments.size() - 1;
    }

    double Loan::getAmount()
    {
        return m_amount;
    }

    std::shared_ptr<utils::JSON::Object> Loan::toJson()
    {
        std::shared_ptr<utils::JSON::Object> object = std::make_shared<utils::JSON::Object>();
        object->setAttribute("amount", float(m_amount));
        object->setAttribute("currentInstallment", int(m_currentInstallment));
        object->setAttribute("interestRate", float(m_interestRate));
        object->setAttribute("endOfContract_year", m_endOfContract.getYear());
        object->setAttribute("endOfContract_month", m_endOfContract.getMonth());
        object->setAttribute("endOfContract_day", m_endOfContract.getDay());

        object->setAttribute("startDay_year", m_startDay.getYear());
        object->setAttribute("startDay_month", m_startDay.getMonth());
        object->setAttribute("startDay_day", m_startDay.getDay());

        return object;
    }
    std::shared_ptr<Loan> Loan::fromJson(const std::shared_ptr<utils::JSON::Object> &object)
    {
        float amount = object->getFloatValue("amount");
        int currentInstallment = object->getIntValue("currentInstallment");
        float interestRate = object->getFloatValue("interestRate");

        utils::time::Date startDay(object->getIntValue("startDay_year"), object->getIntValue("startDay_month"), object->getIntValue("startDay_day"));
        utils::time::Date endOfContract(object->getIntValue("endOfContract_year"), object->getIntValue("endOfContract_month"), object->getIntValue("startDay_day"));

        std::shared_ptr<Loan> loan = std::make_shared<Loan>(amount, interestRate, startDay, endOfContract);

        loan->m_currentInstallment = currentInstallment;
        loan->generateInstallments();

        return loan;
    }
    utils::time::Date &Loan::getStartDay()
    {
        return m_startDay;
    }
    utils::time::Date &Loan::getEndOfContract()
    {
        return m_endOfContract;
    }
}