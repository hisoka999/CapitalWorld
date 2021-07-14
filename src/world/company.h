#ifndef COMPANY_H
#define COMPANY_H

#include <string>
#include <world/building.h>
#include <vector>
#include <memory>
#include <engine/utils/json/object.h>

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

        std::shared_ptr<utils::JSON::Object> toJson();
        static std::shared_ptr<Company> fromJson(const std::shared_ptr<utils::JSON::Object> &object);
        std::vector<std::shared_ptr<world::Building>> &getBuildings();

    private:
        std::string name;
        float cash;
        float income;
        float costs;
        bool player;
        std::vector<std::shared_ptr<world::Building>> buildings;
    };
}
#endif // COMPANY_H
