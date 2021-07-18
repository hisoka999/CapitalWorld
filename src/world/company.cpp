#include "company.h"
#include <algorithm>
#include "services/buildingservice.h"
#include <magic_enum.hpp>
#include "world/buildings/street.h"
namespace world
{

    Company::Company(std::string name, float cash, bool player)
        : name(name), cash(cash), player(player), income(0), costs(0)
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
        return income - costs;
    }

    float Company::getCosts()
    {
        return costs;
    }

    float Company::getIncome()
    {
        return income;
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
        costs = 0;
        income = 0;
        for (auto &building : buildings)
        {
            building->calculateBalance(month, year);
            building->updateProduction(month, year);
            if (building->isAutoSellActive())
                building->autoSell(month, year);

            costs += building->getCostsPerMonth(month, year);

            income += building->getIncomePerMonth(month, year);
        }
        incCash(income - costs);
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

    std::shared_ptr<utils::JSON::Object> Company::toJson()
    {
        std::shared_ptr<utils::JSON::Object> obj = std::make_shared<utils::JSON::Object>();
        obj->setAttribute("name", getName());
        obj->setAttribute("cash", getCash());
        obj->setAttribute("maxBuildingIndex", getMaxBuildingIndex());

        utils::JSON::JsonArray jsonBuildings;
        for (auto &building : buildings)
        {

            utils::JSON::JsonValue jbuilding = building->toJson();
            jsonBuildings.push_back(jbuilding);
        }
        obj->setArrayAttribute("buildings", jsonBuildings);

        return obj;
    }

    std::shared_ptr<Company> Company::fromJson(const std::shared_ptr<utils::JSON::Object> &object)
    {
        std::string name = object->getStringValue("name");
        float cash = object->getFloatValue("cash");
        int maxBuildingIndex = object->getIntValue("maxBuildingIndex");
        auto company = std::make_shared<Company>(name, cash, true);
        auto buildings = object->getArray("buildings");

        for (auto val : buildings)
        {
            auto b = std::get<std::shared_ptr<utils::JSON::Object>>(val);
            world::BuildingType type = magic_enum::enum_cast<world::BuildingType>(b->getStringValue("type")).value();
            switch (type)
            {
            case world::BuildingType::Street:
            {
                auto street = std::make_shared<world::buildings::Street>();
                //todo change position, or move to new class
                graphics::Rect rect;
                rect.x = 0;
                rect.y = 128;
                rect.width = 64;
                rect.height = 32;
                street->setSourceRect(rect);
                street->setOffset(0, 0);
                street->setPosition(b->getFloatValue("pos_x"), b->getFloatValue("pos_y"));
                street->setSubTexture("street1");
                company->addBuilding(street);

                break;
            }
            case world::BuildingType::Transport:
            default:
                auto building = services::BuildingService::Instance().find(type);
                company->addBuilding(Building::fromJson(building, b, company.get()));
            }
        }

        return company;
    }

    std::vector<std::shared_ptr<world::Building>> &Company::getBuildings()
    {
        return buildings;
    }

    std::shared_ptr<world::Building> Company::findBuildingByDisplayName(const std::string &name)
    {
        for (auto &building : buildings)
        {
            if (building->getDisplayName() == name)
            {
                return building;
            }
        }
        return nullptr;
    }

}
