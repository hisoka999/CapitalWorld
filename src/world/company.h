#ifndef COMPANY_H
#define COMPANY_H

#include "research.h"
#include <string>
#include <world/building.h>

#include <engine/utils/json/object.h>
#include <engine/utils/time/date.h>
#include <memory>
#include <vector>

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
        std::shared_ptr<world::Building> findBuildingByDisplayName(const std::string &name);
        std::vector<std::shared_ptr<world::Building>> findAvailableBuildingsByType(world::BuildingType type);

        void research();

        std::vector<std::shared_ptr<Research>> getResearchQueue() const;
        void addResearchToQueue(const std::shared_ptr<Research> &research);
        std::vector<std::shared_ptr<Research>> &getAvailableResearch();
        void setAvailableResearch(const std::vector<std::shared_ptr<Research>> &list);
        int getResearchPerMonth();

    private:
        std::string name;
        float cash;
        float income;
        float costs;
        bool player;
        std::vector<std::shared_ptr<world::Building>> buildings;
        std::vector<std::shared_ptr<Research>> researchQueue;
        std::vector<std::shared_ptr<Research>> availableResearch;
    };
}
#endif // COMPANY_H
