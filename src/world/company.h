#ifndef COMPANY_H
#define COMPANY_H

#include "research.h"
#include <string>
#include <world/building.h>

#include "loan.h"
#include <engine/utils/json/object.h>
#include <engine/utils/time/date.h>
#include <memory>
#include <vector>
#include "balance.h"

namespace world
{
    namespace actions
    {
        class Action;
    };
};

struct KeyValue
{
    std::string key;
    std::string value;
};

namespace world
{

    enum class CompanyColor
    {
        black,
        blue,
        green,
        grey,
        orange,
        red,
        white,
        yellow
    };

    class Company
    {
    public:
        Company(std::string name, double cash, bool player, CompanyColor color);
        Company(Company &other) = delete;
        ~Company();

        std::string getName();
        double getCash();
        double getMonthlyProfit() const;
        double getYearlyProfit(int year);
        double getCosts();
        double getIncome();
        void incCash(double value);
        bool isPLayer();
        void addBuilding(std::shared_ptr<Building> building);
        bool hasBuilding(std::shared_ptr<Building> building);
        void removeBuilding(std::shared_ptr<Building> building);
        void updateBalance(int month, int year);
        void updateDaily(int day, int month, int year);
        std::vector<std::shared_ptr<Building>> findProductionBuildings();
        int getMaxBuildingIndex();
        CompanyColor getColor();

        std::shared_ptr<utils::JSON::Object> toJson();
        static std::shared_ptr<Company> fromJson(const std::shared_ptr<utils::JSON::Object> &object);
        std::vector<std::shared_ptr<world::Building>> &getBuildings();
        std::shared_ptr<world::Building> findBuildingByDisplayName(const std::string &name);
        std::vector<std::shared_ptr<world::Building>> findAvailableBuildingsByType(world::BuildingType type);
        std::vector<std::shared_ptr<world::Building>> findBuildingsByType(world::BuildingType type);
        std::vector<std::shared_ptr<Product>> findAvialableBaseProducts(world::BuildingType type);
        std::vector<std::shared_ptr<Product>> findAvailableProducts();
        std::vector<std::shared_ptr<ProductBalance>> getAccountBalanceForYear(int year);

        void research();

        std::vector<std::shared_ptr<Research>> getResearchQueue() const;
        void addResearchToQueue(const std::shared_ptr<Research> &research);
        std::vector<std::shared_ptr<Research>> &getAvailableResearch();
        std::vector<std::shared_ptr<Research>> getUnresearchedResearch();
        void setAvailableResearch(const std::vector<std::shared_ptr<Research>> &list);
        int getResearchPerMonth();

        std::shared_ptr<world::actions::Action> currentAction();
        void setCurrentAction(const std::shared_ptr<world::actions::Action> &action);
        double calculateCompanyValue();
        void addLoan(const std::shared_ptr<world::Loan> &loan);
        size_t numberOfLoans();
        std::vector<std::shared_ptr<world::Loan>> &getActiveLoans();
        std::vector<KeyValue> displayData();

    private:
        std::string name;
        double cash;
        bool player;
        CompanyColor color;
        double income;
        double costs;
        std::vector<std::shared_ptr<world::Building>> buildings;
        std::vector<std::shared_ptr<Research>> researchQueue;
        std::vector<std::shared_ptr<Research>> availableResearch;
        std::shared_ptr<world::actions::Action> m_currentAction = nullptr;
        std::vector<std::shared_ptr<world::Loan>> m_activeLoans;
        Balance m_balance;
    };
}
#endif // COMPANY_H
