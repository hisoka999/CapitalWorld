#include "company.h"
#include "services/buildingservice.h"
#include "services/productservice.h"
#include "services/researchservice.h"
#include "world/buildings/WorkerComponent.h"
#include "world/buildings/street.h"
#include <algorithm>
#include <magic_enum.hpp>
#include "translate.h"
#include "messages.h"
#include "notifications/EventQueue.h"
#include <cassert>

namespace world
{

    Company::Company(std::string name, double cash, bool player, CompanyColor color)
        : name(name), cash(cash), player(player), color(color), income(0), costs(0)
    {
    }
    Company::~Company()
    {
    }

    std::string Company::getName()
    {
        return name;
    }
    double Company::getCash()
    {
        return cash;
    }
    double Company::getMonthlyProfit() const
    {
        return income - costs;
    }

    double Company::getYearlyProfit(int year)
    {

        double profit = 0.0;
        for (auto balance : getAccountBalanceForYear(year))
        {
            profit += balance->income - balance->costs;
        }
        return profit;
    }

    double Company::getCosts()
    {
        return costs;
    }

    double Company::getIncome()
    {
        return income;
    }
    void Company::incCash(double value)
    {
        cash += value;
    }
    bool Company::isPLayer()
    {
        return player;
    }
    void Company::addBuilding(std::shared_ptr<Building> building)
    {
        assert(building != nullptr);
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
            if (building == nullptr)
                continue;
            building->getBalance().calculateBalance(month, year, building->getProducts());
            building->delayedUpdate(this);
            building->updateProduction(month, year);
            if (building->isAutoSellActive())
                building->autoSell(month, year);

            if (building != nullptr)
            {

                costs += building->getBalance().getCostsPerMonth(month, year);

                income += building->getBalance().getIncomePerMonth(month, year);
            }
        }
        auto iter = m_activeLoans.begin();
        while (iter != m_activeLoans.end())
        {
            std::shared_ptr<world::Loan> &loan = *iter;
            world::Installment installment;
            bool repayed = loan->repayInstallment(installment);
            m_balance.addCosts(month, year, "", BalanceAccount::Interest, installment.interest);
            m_balance.addCosts(month, year, "", BalanceAccount::Repayment, installment.installmentAmount);

            if (repayed)
            {
                iter = m_activeLoans.erase(iter);
                if (player)
                {
                    const notifications::Event event = {notifications::EventType::Loan, _("Loan"), _("Loan has been repayed.")};
                    core::MessageSystem<MessageTypes>::get().sendMessage<notifications::Event>(MessageTypes::Event, event);
                }
            }
            else
            {
                iter++;
            }
        }
        costs += m_balance.getCostsPerMonth(month, year);

        income += m_balance.getIncomePerMonth(month, year);
        incCash(income - costs);
        research();

        if (getCash() < 0 && player)
        {
            notifications::Event event = {notifications::EventType::Balance, _("Cash"), _("Your cash balance is negative.")};
            core::MessageSystem<MessageTypes>::get().sendMessage<notifications::Event>(MessageTypes::Event, event);
        }
    }

    void Company::updateDaily(int day, int month, int year)
    {
        for (auto &building : buildings)
        {
            if (building == nullptr)
                continue;

            building->updateDaily(day, month, year, this);
        }
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

    CompanyColor Company::getColor()
    {
        return color;
    }

    std::shared_ptr<utils::JSON::Object> Company::toJson()
    {
        std::shared_ptr<utils::JSON::Object> obj = std::make_shared<utils::JSON::Object>();
        obj->setAttribute("name", getName());
        obj->setAttribute("cash", getCash());
        obj->setAttribute("maxBuildingIndex", getMaxBuildingIndex());
        obj->setAttribute("player", player);
        obj->setAttribute("color", std::string(magic_enum::enum_name(getColor())));

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

        utils::JSON::JsonArray activeLoans;
        for (auto &loan : m_activeLoans)
        {
            activeLoans.push_back(loan->toJson());
        }

        obj->setArrayAttribute("active_loans", activeLoans);

        return obj;
    }

    std::shared_ptr<Company> Company::fromJson(const std::shared_ptr<utils::JSON::Object> &object)
    {
        std::string name = object->getStringValue("name");
        float cash = object->getFloatValue("cash");
        bool player = object->getBoolValue("player");
        CompanyColor color = magic_enum::enum_cast<world::CompanyColor>(object->getStringValue("color")).value();
        // int maxBuildingIndex = object->getIntValue("maxBuildingIndex");
        auto company = std::make_shared<Company>(name, cash, player, color);
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

        if (object->hasArray("active_loans"))
        {
            utils::JSON::JsonArray activeLoans = object->getArray("active_loans");

            for (auto &jsonLoan : activeLoans)
            {
                auto obj = std::get<std::shared_ptr<utils::JSON::Object>>(jsonLoan);
                company->m_activeLoans.push_back(Loan::fromJson(obj));
            }
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
            for (auto &balance : building->getBalance().getBalancePerYear(year))
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

        for (auto &balance : m_balance.getBalancePerYear(year))
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

    std::vector<std::shared_ptr<Research>> Company::getUnresearchedResearch()
    {
        std::vector<std::shared_ptr<Research>> result;
        for (auto &res : availableResearch)
        {
            if (!res->getResearched())
                result.push_back(res);
        }
        return result;
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
        value += getMonthlyProfit();

        for (auto &loan : m_activeLoans)
        {
            value -= loan->calculateRepaymentWithInterest();
        }
        return value;
    }

    void Company::addLoan(const std::shared_ptr<world::Loan> &loan)
    {
        m_activeLoans.push_back(loan);
        cash += loan->getAmount();
    }

    size_t Company::numberOfLoans()
    {
        return m_activeLoans.size();
    }

    std::vector<std::shared_ptr<world::Loan>> &Company::getActiveLoans()
    {
        return m_activeLoans;
    }

    std::vector<KeyValue> Company::displayData()
    {
        std::vector<KeyValue> dataMap;
        dataMap.push_back({_("Company Name: "), getName()});
        dataMap.push_back({_("Cash: "), format_currency(getCash())});
        dataMap.push_back({_("Company Value: "), format_currency(calculateCompanyValue())});
        dataMap.push_back({_("Number of Buildings: "), utils::string_format("%i", getBuildings().size())});

        return dataMap;
    }
    void Company::research()
    {
        if (researchQueue.size() == 0)
            return;

        auto &currentResearch = researchQueue.front();

        currentResearch->reduceCosts(getResearchPerMonth());
        if (currentResearch->getResearched())
        {
            if (player)
            {
                notifications::Event event = {notifications::EventType::Research, _("Research"), utils::string_format(_("%s was researched."), currentResearch->getName())};
                auto msg = std::make_unique<core::Message<MessageTypes, notifications::Event>>(MessageTypes::Event, event);
                core::MessageSystem<MessageTypes>::get().sendMessage<notifications::Event>(MessageTypes::Event, event);
            }
            researchQueue.erase(researchQueue.begin());

            if (researchQueue.empty() && player)
            {
                notifications::Event event = {notifications::EventType::Research, _("Research"), _("Research queue is empty.")};
                auto msg = std::make_unique<core::Message<MessageTypes, notifications::Event>>(MessageTypes::Event, event);
                core::MessageSystem<MessageTypes>::get().sendMessage<notifications::Event>(MessageTypes::Event, event);
            }
        }
    }
}
