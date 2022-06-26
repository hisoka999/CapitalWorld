#include "company.h"
#include "services/buildingservice.h"
#include "services/productservice.h"
#include "services/researchservice.h"
#include "world/buildings/WorkerComponent.h"
#include "world/buildings/street.h"
#include <algorithm>
#include <magic_enum.hpp>

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

        for (auto iter = activeLoans.begin(); iter != activeLoans.end(); iter++)
        {
            auto loan = *iter;
            world::Installment installment;
            bool repayed = loan.repayInstallment(installment);

            if (repayed)
            {
                activeLoans.erase(iter);
            }
        }

        incCash(income - costs);
        research();
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

        std::shared_ptr<utils::JSON::Object> jsonResearch = std::make_shared<utils::JSON::Object>();

        for (auto &res : availableResearch)
        {
            jsonResearch->setAttribute(res->getName(), res->getResearched());
        }
        obj->setAttribute("research", jsonResearch);

        return obj;
    }

    std::shared_ptr<Company> Company::fromJson(const std::shared_ptr<utils::JSON::Object> &object)
    {
        std::string name = object->getStringValue("name");
        float cash = object->getFloatValue("cash");
        // int maxBuildingIndex = object->getIntValue("maxBuildingIndex");
        auto company = std::make_shared<Company>(name, cash, true);
        auto buildings = object->getArray("buildings");

        for (auto val : buildings)
        {
            auto b = std::get<std::shared_ptr<utils::JSON::Object>>(val);
            world::BuildingType type = magic_enum::enum_cast<world::BuildingType>(b->getStringValue("type")).value();
            std::string name = b->getStringValue("name");
            switch (type)
            {
            case world::BuildingType::Street:
            {
                auto street = std::make_shared<world::buildings::Street>();
                // todo change position, or move to new class
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
                auto building = services::BuildingService::Instance().findByName(name);
                company->addBuilding(Building::fromJson(building, b, company.get()));
            }
        }

        for (auto &building : company->buildings)
        {
            building->delayedUpdate(company.get());
        }

        company->setAvailableResearch(services::ResearchService::Instance().getData());

        auto research = object->getObjectValue("research");

        for (auto researchName : research->getAttributes())
        {
            bool researched = research->getBoolValue(researchName);

            for (auto &r : company->getAvailableResearch())
            {
                if (r->getName() == researchName)
                    r->setResearched(researched);
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

    std::vector<std::shared_ptr<world::Building>> Company::findAvailableBuildingsByType(world::BuildingType type)
    {
        auto buildings = services::BuildingService::Instance().find(type);
        std::vector<std::shared_ptr<world::Building>> result;
        for (auto &building : buildings)
        {
            bool canBuild = true;
            for (auto research : availableResearch)
            {
                if (research->canEnableObject(building->getName()) && !research->getResearched())
                {
                    canBuild = false;
                    break;
                }
            }
            if (canBuild)
            {
                result.push_back(building);
            }
        }
        return result;
    }

    std::vector<std::shared_ptr<world::Building>> Company::findBuildingsByType(world::BuildingType type)
    {
        std::vector<std::shared_ptr<world::Building>> result;
        for (auto &building : buildings)
        {
            if (building->getType() == type)
                result.push_back(building);
        }
        return result;
    }

    std::vector<std::shared_ptr<Product>> Company::findAvialableBaseProducts(world::BuildingType type)
    {
        auto products = services::ProductService::Instance().getBaseProductsByBuildingType(type);

        std::vector<std::shared_ptr<Product>> result;

        for (auto &product : products)
        {
            bool canProduce = true;
            for (auto research : availableResearch)
            {
                if (research->canEnableObject(product->getName()) && !research->getResearched())
                {
                    canProduce = false;
                    break;
                }
            }
            if (canProduce)
            {
                result.push_back(product);
            }
        }
        return result;
    }

    std::vector<std::shared_ptr<Product>> Company::findAvailableProducts()
    {
        auto products = services::ProductService::Instance().getData();
        std::vector<std::shared_ptr<Product>> result;

        for (auto &product : products)
        {
            bool canProduce = true;
            for (auto research : availableResearch)
            {
                if (research->canEnableObject(product->getName()) && !research->getResearched())
                {
                    canProduce = false;
                    break;
                }
            }
            if (canProduce)
            {
                result.push_back(product);
            }
        }
        return result;
    }

    std::vector<std::shared_ptr<ProductBalance>> Company::getAccountBalanceForYear(int year)
    {
        std::map<BalanceAccount, std::shared_ptr<ProductBalance>> balanceMap;
        std::vector<std::shared_ptr<ProductBalance>> balanceList;
        for (auto &building : buildings)
        {
            for (auto &balance : building->getBalancePerYear(year))
            {
                if (balanceMap.count(balance.account) > 0)
                {
                    balanceMap[balance.account]->costs += balance.costs;
                    balanceMap[balance.account]->income += balance.income;
                }
                else
                {
                    std::shared_ptr<ProductBalance> b = std::make_shared<ProductBalance>();
                    b->account = balance.account;
                    b->costs = balance.costs;
                    b->income = balance.income;
                    b->year = year;
                    balanceMap[balance.account] = b;
                }
            }
        }
        for (auto val : balanceMap)
        {
            balanceList.push_back(val.second);
        }
        return balanceList;
    }
    std::vector<std::shared_ptr<Research>> Company::getResearchQueue() const
    {
        return researchQueue;
    }

    void Company::addResearchToQueue(const std::shared_ptr<Research> &research)
    {
        auto found = std::find(researchQueue.begin(), researchQueue.end(), research);
        if (found == std::end(researchQueue))
        {
            researchQueue.push_back(research);
        }
    }

    std::vector<std::shared_ptr<Research>> &Company::getAvailableResearch()
    {
        return availableResearch;
    }

    void Company::setAvailableResearch(const std::vector<std::shared_ptr<Research>> &list)
    {
        this->availableResearch.clear();
        for (auto &research : list)
        {
            this->availableResearch.push_back(std::make_shared<Research>(*research));
        }

        for (auto &research : availableResearch)
        {

            auto names = research->getRequirementNames();
            for (auto name : names)
            {
                for (auto &sub : availableResearch)
                {
                    if (sub->getName() == name)
                    {
                        research->addRequirement(sub);
                        break;
                    }
                }
            }
        }
    }

    int Company::getResearchPerMonth()
    {
        int research = 0;
        for (auto &building : buildings)
        {
            if (building->getName() == "ResearchLab" && building->hasComponent("WorkerComponent"))
            {
                auto component = building->getComponent<world::buildings::WorkerComponent>("WorkerComponent");
                research += component->getCurrentWorkers();
            }
        }
        return research;
    }

    std::shared_ptr<world::actions::Action> Company::currentAction()
    {
        return m_currentAction;
    }

    void Company::setCurrentAction(const std::shared_ptr<world::actions::Action> &action)
    {
        m_currentAction = action;
    }

    double Company::calculateCompanyValue()
    {
        double value = 0.0;
        for (auto &building : buildings)
        {
            value += (building->getBuildPrice() * 0.5);
        }
        value += getProfit();
        for (auto &loan : activeLoans)
        {
            value -= loan.calculateRepaymentWithInterest();
        }
        return value;
    }
    void Company::research()
    {
        if (researchQueue.size() == 0)
            return;

        auto &currentResearch = researchQueue.front();

        currentResearch->reduceCosts(getResearchPerMonth());
        if (currentResearch->getResearched())
        {
            researchQueue.erase(researchQueue.begin());
        }
    }

}
