#include "company.h"
#include <algorithm>

namespace world
{

    Company::Company(std::string name, float cash, bool player)
        : name(name), cash(cash), profit(0), player(player)
    {
    }
    Company::~Company()
    {
    }

    std::string Company::getName()
    {
        return name;
    }
    float Company::getCash()
    {
        return cash;
    }
    float Company::getProfit()
    {
        return profit;
    }
    void Company::incCash(float value)
    {
        cash += value;
    }
    bool Company::isPLayer()
    {
        return player;
    }
    void Company::addBuilding(std::shared_ptr<Building> building)
    {
        buildings.push_back(building);
    }
    bool Company::hasBuilding(std::shared_ptr<Building> building)
    {
        auto it = std::find(buildings.begin(), buildings.end(), building);
        return it != buildings.end();
    }
    void Company::removeBuilding(std::shared_ptr<Building> building)
    {
        auto it = std::find(buildings.begin(), buildings.end(), building);
        if (it != buildings.end())
        {
            buildings.erase(it);
        }
    }

    void Company::updateBalance(int month, int year)
    {
        profit = 0.0f;
        for (auto &building : buildings)
        {
            building->calculateBalance(month, year);
            building->updateProduction(month, year);
            if (building->isAutoSellActive())
                building->autoSell(month, year);
            profit -= building->getCostsPerMonth(month, year);

            profit += building->getIncomePerMonth(month, year);
        }
        incCash(profit);
    }

    std::vector<std::shared_ptr<Building>> Company::findProductionBuildings()
    {
        std::vector<std::shared_ptr<Building>> productionBuildings;
        for (auto &building : buildings)
        {
            if (building->getProducts().size() > 0)
            {
                productionBuildings.push_back(building);
            }
        }
        return productionBuildings;
    }

    int Company::getMaxBuildingIndex()
    {
        return buildings.size();
    }

}
