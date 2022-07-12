#pragma once

#include <string>
#include <vector>
#include <memory>
#include "product.h"
#include <engine/utils/json/object.h>

namespace world
{

    enum class BalanceAccount
    {
        Production,
        Transport,
        Import,
        Research,
        Sales,
        Interest,
        Repayment
    };
    struct ProductBalance
    {
        int month;
        int year;
        std::string name;
        double costs;
        double income;
        BalanceAccount account;
    };
    class Balance
    {
    public:
        Balance(/* args */);
        ~Balance();
        void addCosts(int month, int year, const std::string &productName, BalanceAccount account, int amount);
        void addIncome(int month, int year, const std::string &productName, BalanceAccount account, int amount);
        std::vector<ProductBalance> getBalancePerYear(int year);
        std::vector<std::shared_ptr<ProductBalance>> getAccountBalanceForYear(int year);

        void addBalance(ProductBalance value);
        void calculateBalance(int month, int year, std::vector<std::shared_ptr<Product>> products);
        float getCostsPerMonth(int month, int year);
        float getIncomePerMonth(int month, int year);
        virtual std::shared_ptr<utils::JSON::Object> toJson();
        static Balance fromJson(const std::shared_ptr<utils::JSON::Object> &object);

    private:
        std::vector<ProductBalance> m_balance;
    };

} // namespace world
