#include "gtest/gtest.h"

#include <engine/utils/time/date.h>
#include <world/loan.h>

TEST(LoanTestTest, GenerateLoan)
{
    double amount = 5000000;
    double interestRate = 5;
    utils::time::Date currentDay(2022, 6, 25);
    utils::time::Date endOfContract(2032, 5, 1);

    world::Loan loan(amount, interestRate, currentDay, endOfContract);

    EXPECT_EQ(118, loan.calculateNumberOfInstallments());
    auto installments = loan.getInstallments();

    EXPECT_EQ(118, installments.size());
    EXPECT_EQ(20656.78, installments[0].interest);
    EXPECT_EQ(0, installments[installments.size() - 1].interest);
    double caulatedAmount = 0;
    for (auto &installment : installments)
    {
        EXPECT_EQ(42372.88, installment.installmentAmount);
        caulatedAmount += installment.installmentAmount;
    }
}

TEST(LoanTestTest, GenerateLoanOtherBase)
{
    double amount = 3500000;
    double interestRate = 2.5;
    utils::time::Date currentDay(2022, 6, 25);
    utils::time::Date endOfContract(2025, 1, 1);

    world::Loan loan(amount, interestRate, currentDay, endOfContract);

    EXPECT_EQ(30, loan.calculateNumberOfInstallments());
    auto installments = loan.getInstallments();

    EXPECT_EQ(30, installments.size());
    EXPECT_EQ(7048.61, installments[0].interest);
    EXPECT_EQ(0, installments[installments.size() - 1].interest);
    double caulatedAmount = 0;
    for (auto &installment : installments)
    {
        EXPECT_EQ(116666.67, installment.installmentAmount);
        caulatedAmount += installment.installmentAmount;
    }
}
