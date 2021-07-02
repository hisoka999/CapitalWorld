#ifndef COMPANY_H
#define COMPANY_H

#include <string>
#include <world/building.h>
#include <vector>
#include <memory>

namespace world
{

    class Company
    {
    public:
        Company(std::string name, float cash, bool player);
        ~Company();

        std::string getName();
        float getCash();
        float getProfit();
        float getCosts();
        float getIncome();
        void incCash(float value);
        bool isPLayer();
        void addBuilding(std::shared_ptr<Building> building);
        bool hasBuilding(std::shared_ptr<Building> building);
        void removeBuilding(std::shared_ptr<Building> building);
        void updateBalance(int month, int year);
        std::vector<std::shared_ptr<Building>> findProductionBuildings();
        int getMaxBuildingIndex();

    private:
        std::string name;
        float cash;
        float profit;
        float income;
        float costs;
        bool player;
        std::vector<std::shared_ptr<world::Building>> buildings;
    };
}
#endif // COMPANY_H
