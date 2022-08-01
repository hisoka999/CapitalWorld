#include "balance.h"
#include <magic_enum.hpp>

namespace world
{
    Balance::Balance(/* args */)
    {
    }

    Balance::~Balance()
    {
    }

    float Balance::getIncomePerMonth(int month, int year)
    {
        float result = .0f;
        for (auto &b : m_balance)
        {
            if (b.month == month && b.year == year)
            {
                result += b.income;
            }
        }
        return result;
    }

    std::shared_ptr<utils::JSON::Object> Balance::toJson()
    {
        std::shared_ptr<utils::JSON::Object> myBuilding = std::make_shared<utils::JSON::Object>();

        utils::JSON::JsonArray balanceArray;
        for (auto &b : m_balance)
        {
            std::shared_ptr<utils::JSON::Object> jBalance = std::make_shared<utils::JSON::Object>();
            jBalance->setAttribute("year", b.year);
            jBalance->setAttribute("month", b.month);
            jBalance->setAttribute("product", b.name);
            jBalance->setAttribute("costs", float(b.costs));
            jBalance->setAttribute("income", float(b.income));
            jBalance->setAttribute("account", std::string(magic_enum::enum_name(b.account)));
            utils::JSON::JsonValue value = jBalance;
            balanceArray.push_back(value);
        }

        myBuilding->setArrayAttribute("balance", balanceArray);
        return myBuilding;
    }

    Balance Balance::fromJson(const std::shared_ptr<utils::JSON::Object> &object)
    {
        Balance balance;
        utils::JSON::JsonArray balanceArray = object->getArray("balance");
        for (auto b : balanceArray)
        {
            auto balanceObject = std::get<std::shared_ptr<utils::JSON::Object>>(b);
            ProductBalance productBalance;
            productBalance.account = magic_enum::enum_cast<BalanceAccount>(balanceObject->getStringValue("account")).value();
            productBalance.costs = balanceObject->getFloatValue("costs");
            productBalance.income = balanceObject->getFloatValue("income");
            productBalance.month = balanceObject->getIntValue("month");
            productBalance.year = balanceObject->getIntValue("year");
            productBalance.name = balanceObject->getStringValue("product");
            balance.addBalance(productBalance);
        }
        return balance;
    }

    float Balance::getCostsPerMonth(int month, int year)
    {
        float result = .0f;
        for (auto &b : m_balance)
        {
            if (b.month == month && b.year == year)
            {
                result += b.costs;
            }
        }
        return result;
    }

    void Balance::addCosts(int month, int year, const std::string &productName, BalanceAccount account, int amount)
    {

        bool found = false;
        for (int i = m_balance.size() - 1; i >= 0; i--)
        {
            auto &b = m_balance[i];
            if (b.name == productName && b.year == year && b.month == month && b.account == account)
            {
                b.costs += amount;
                found = true;
                break;
            }
        }
        if (!found)
        {
            ProductBalance productBalance;
            productBalance.name = productName;
            productBalance.year = year;
            productBalance.month = month;
            productBalance.costs = amount;
            productBalance.income = 0;
            productBalance.account = account;
            m_balance.push_back(productBalance);
        }
    }

    void Balance::addIncome(int month, int year, const std::string &productName, BalanceAccount account, int amount)
    {
        bool found = false;
        // for (auto &b : m_balance)
        for (int i = m_balance.size() - 1; i >= 0; i--)
        {
            auto &b = m_balance[i];
            if (b.name == productName && b.year == year && b.month == month && b.account == account)
            {
                b.income += amount;
                found = true;
                break;
            }
        }
        if (!found)
        {
            ProductBalance productBalance;
            productBalance.name = productName;
            productBalance.year = year;
            productBalance.month = month;
            productBalance.income = amount;
            productBalance.costs = 0;
            productBalance.account = account;
            m_balance.push_back(productBalance);
        }
    }
    void Balance::addBalance(ProductBalance value)
    {
        m_balance.push_back(value);
    }

    void Balance::calculateBalance(int month, int year, std::vector<std::shared_ptr<Product>> products)
    {
        for (auto &product : products)
        {
            ProductBalance productBalance;
            productBalance.name = product->getName();
            productBalance.year = year;
            productBalance.month = month;
            productBalance.costs = product->calculateCostsPerMonth();
            productBalance.income = 0; // TODO
            productBalance.account = BalanceAccount::Production;
            m_balance.push_back(productBalance);
        }
    }
    std::vector<ProductBalance> Balance::getBalancePerYear(int year)
    {
        std::vector<ProductBalance> result;
        for (auto b : m_balance)
        {
            if (b.year == year)
            {
                result.push_back(b);
            }
        }
        return result;
    }
}