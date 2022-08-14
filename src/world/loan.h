#pragma once

#include <engine/utils/time/date.h>
#include <vector>
#include <engine/utils/json/object.h>

namespace world
{

    struct Installment
    {
        utils::time::Date dueDate;
        double installmentAmount;
        double interest;
        Installment(utils::time::Date dueDate,
                    double installmentAmount,
                    double interest)
            : dueDate(dueDate), installmentAmount(installmentAmount), interest(interest)
        {
        }
        Installment()
            : dueDate(1900, 1, 1)
        {
        }
    };

    class Loan
    {
    public:
        Loan(double amount, double interestRate, utils::time::Date startDay, utils::time::Date endOfContract);
        ~Loan();
        std::vector<Installment> getInstallments();
        size_t calculateNumberOfInstallments();
        double calculateRepaymentWithInterest();
        double calculateRepayment();
        double calculateInterest();
        bool repayInstallment(Installment &installment);
        double getAmount();
        utils::time::Date &getStartDay();
        utils::time::Date &getEndOfContract();
        virtual std::shared_ptr<utils::JSON::Object> toJson();
        static std::shared_ptr<Loan> fromJson(const std::shared_ptr<utils::JSON::Object> &object);

    protected:
        void generateInstallments();

    private:
        double m_amount;
        double m_interestRate;
        utils::time::Date m_startDay;
        utils::time::Date m_endOfContract;
        std::vector<Installment> m_installments;
        size_t m_currentInstallment = 0;
    };

} // namespace world
