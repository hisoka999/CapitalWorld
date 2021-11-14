#include "building.h"
#include "buildings/HouseComponent.h"
#include "buildings/SalesComponent.h"
#include "buildings/StorageComponent.h"
#include "buildings/TransportComponent.h"
#include "buildings/WorkerComponent.h"
#include "services/productservice.h"
#include <algorithm>
#include <magic_enum.hpp>

namespace world
{

    std::map<std::string, std::shared_ptr<world::buildings::BuildingComponent>> Building::componentMap;

    Building::Building(std::string name, std::string displayName, std::string description, int buildPrice, BuildingType type, int blockWidth, int blockHeight)
        : name(name), displayName(displayName), type(type), description(description), buildPrice(buildPrice), blockWidth(blockWidth), blockHeight(blockHeight), xOffset(0), yOffset(0)
    {
    }

    Building::Building(const Building &copy)
    {
        this->name = copy.name;
        this->displayName = copy.displayName;
        this->description = copy.description;
        this->buildPrice = copy.buildPrice;
        this->type = copy.type;
        this->blockWidth = copy.blockWidth;
        this->blockHeight = copy.blockHeight;
        this->xOffset = copy.xOffset;
        this->yOffset = copy.yOffset;
        this->displayRect = copy.displayRect;
        this->sourceRect = copy.sourceRect;
        this->hashId = copy.hashId;
        for (auto &component : copy.components)
        {
            components[component.first] = component.second->clone();
        }

        for (auto res : copy.rawResources)
        {
            rawResources.push_back(res);
        }
    }

    Building::Building(std::string name, std::string displayName, std::string description, int buildPrice, BuildingType type)
        : name(name), displayName(displayName), type(type), description(description), buildPrice(buildPrice), blockWidth(1), blockHeight(1), xOffset(0), yOffset(0)
    {
    }

    BuildingType Building::getType()
    {
        return type;
    }

    std::string Building::getName()
    {
        return name;
    }
    std::string Building::getDescription()
    {
        return description;
    }

    const std::string &Building::getDisplayName() const
    {
        return displayName;
    }

    void Building::setDisplayName(const std::string name)
    {
        displayName = name;
    }
    int Building::getBuildPrice()
    {
        return buildPrice;
    }

    bool Building::canBuild(float money)
    {
        return buildPrice <= money;
    }

    graphics::Rect &Building::getDisplayRect()
    {
        return displayRect;
    }
    graphics::Rect &Building::getSourceRect()
    {
        return sourceRect;
    }
    void Building::setSourceRect(graphics::Rect rect)
    {
        sourceRect = rect;
    }
    void Building::setPosition(float x, float y)
    {
        displayRect.x = x;
        displayRect.y = y;
        displayRect.width = sourceRect.width;
        displayRect.height = sourceRect.height;
    }

    graphics::Rect Building::get2DPosition()
    {
        graphics::Rect r = displayRect;
        r.width = blockWidth;
        r.height = blockHeight;
        return r;
    }
    int Building::getXOffset()
    {
        return xOffset;
    }
    int Building::getYOffset()
    {
        return yOffset;
    }
    void Building::setOffset(int x, int y)
    {
        xOffset = x;
        yOffset = y;
    }
    void Building::addProduct(std::shared_ptr<Product> product)
    {
        if (!hasProduct(product))
            products.push_back(product);
    }
    void Building::removeProduct(std::shared_ptr<Product> product)
    {
        auto it = std::find(products.begin(), products.end(), product);
        if (it != products.end())
        {
            products.erase(it);
        }
    }
    std::vector<std::shared_ptr<Product>> Building::getProducts()
    {
        return products;
    }

    bool Building::hasProduct(std::shared_ptr<Product> product)
    {
        auto it = std::find(products.begin(), products.end(), product);
        return it != products.end();
    }
    void Building::calculateBalance(int month, int year)
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
            balance.push_back(productBalance);
        }
    }
    float Building::getCostsPerMonth(int month, int year)
    {
        float result = .0f;
        for (auto &b : balance)
        {
            if (b.month == month && b.year == year)
            {
                result += b.costs;
            }
        }
        return result;
    }

    float Building::getIncomePerMonth(int month, int year)
    {
        float result = .0f;
        for (auto &b : balance)
        {
            if (b.month == month && b.year == year)
            {
                result += b.income;
            }
        }
        return result;
    }
    void Building::updateProduction(int month, int year)
    {

        if (hasComponent("StorageComponent"))
        {
            auto storage = getComponent<world::buildings::StorageComponent>("StorageComponent");

            for (auto &product : products)
            {

                auto cycle = product->getProductionCycle();
                if (month >= cycle.startMonth && month <= cycle.endMonth)
                {
                    if (product->getBaseProducts().size() > 0)
                    {
                        bool requirementsFullfilled = true;
                        for (auto &base : product->getBaseProducts())
                        {
                            int amount = storage->getEntry(base->product->getName());
                            if (amount < base->amount)
                            {
                                requirementsFullfilled = false;
                            }
                        }
                        if (requirementsFullfilled)
                        {
                            for (auto &base : product->getBaseProducts())
                            {
                                storage->addEntry(base->product->getName(), base->amount * -1);
                            }
                            storage->addEntry(product->getName(), cycle.amount);
                            updateProduction(month, year);
                        }
                    }
                    else if (product->getResources().size() > 0)
                    {
                        storage->addEntry(product->getName(), cycle.amount);
                    }
                }
            }
        }

        for (auto &component : components)
        {
            component.second->updateProduction(month, year, this);
        }
    }

    void Building::addCosts(int month, int year, const std::string &productName, BalanceAccount account, int amount)
    {

        bool found = false;
        for (auto &b : balance)
        {
            if (b.name == productName && b.year == year && b.month == month && b.account == account)
            {
                b.costs += amount;
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
            balance.push_back(productBalance);
        }
    }

    void Building::addIncome(int month, int year, const std::string &productName, BalanceAccount account, int amount)
    {
        bool found = false;
        for (auto &b : balance)
        {
            if (b.name == productName && b.year == year && b.month == month && b.account == account)
            {
                b.income += amount;
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
            balance.push_back(productBalance);
        }
    }

    bool Building::isAutoSellActive()
    {
        return false; // type == BuildingType::Factory;
    }

    void Building::autoSell(int month, int year)
    {
        if (hasComponent("StorageComponent"))
        {
            auto storage = getComponent<world::buildings::StorageComponent>("StorageComponent");

            for (auto &product : products)
            {
                unsigned amount = storage->getEntry(product->getName());
                double income = amount * product->calculateCostsPerPiece() * 1.5;

                // find balance
                for (auto &b : balance)
                {
                    if (b.name == product->getName() && b.year == year && b.month == month && b.account == BalanceAccount::Production)
                    {
                        b.income += income;
                    }
                }
                storage->addEntry(product->getName(), amount * -1);
            }
        }
    }

    std::map<std::string, std::string> Building::displayData()
    {
        std::map<std::string, std::string> result;
        for (auto &component : components)
        {
            for (auto data : component.second->displayData())
            {
                result[data.first] = data.second;
            }
        }
        return result;
    }

    const std::string &Building::getSubTexture()
    {
        return subTexture;
    }

    const size_t Building::getSubTextureHash()
    {
        return hashId;
    }
    void Building::setSubTexture(const std::string &tex)
    {
        subTexture = tex;
        if (!tex.empty())
            hashId = hasher(subTexture);
    }

    std::shared_ptr<utils::JSON::Object> Building::toJson()
    {
        std::shared_ptr<utils::JSON::Object> myBuilding = std::make_shared<utils::JSON::Object>();
        myBuilding->setAttribute("name", getName());
        myBuilding->setAttribute("displayName", getDisplayName());
        myBuilding->setAttribute("pos_x", get2DPosition().x);
        myBuilding->setAttribute("pos_y", get2DPosition().y);
        myBuilding->setAttribute("offset_x", xOffset);
        myBuilding->setAttribute("offset_y", yOffset);
        myBuilding->setAttribute("subTexture", subTexture);
        myBuilding->setAttribute("type", std::string(magic_enum::enum_name(type)));

        utils::JSON::JsonArray productsArray;
        for (auto &product : products)
        {

            utils::JSON::JsonValue value = product->getName();
            productsArray.push_back(value);
        }
        myBuilding->setArrayAttribute("products", productsArray);

        utils::JSON::JsonArray balanceArray;
        for (auto &b : balance)
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

        utils::JSON::JsonArray componentsArray;
        for (auto &component : components)
        {
            utils::JSON::JsonValue value = component.second->toJson();
            componentsArray.push_back(value);
        }
        myBuilding->setArrayAttribute("components", componentsArray);

        return myBuilding;
    }

    std::shared_ptr<Building> Building::fromJson(const std::shared_ptr<Building> &reference, const std::shared_ptr<utils::JSON::Object> &object, world::Company *company)
    {
        auto result = std::make_shared<Building>(*reference);
        result->setDisplayName(object->getStringValue("displayName"));
        int posX = object->getFloatValue("pos_x");
        int posY = object->getFloatValue("pos_y");
        result->setPosition(posX, posY);
        result->setSubTexture(object->getStringValue("subTexture"));
        // add products
        utils::JSON::JsonArray productsArray = object->getArray("products");

        for (auto p : productsArray)
        {
            auto productName = std::get<std::string>(p);
            auto product = services::ProductService::Instance().getProductByName(productName);
            result->addProduct(product);
        }
        // add balance
        utils::JSON::JsonArray balanceArray = object->getArray("balance");
        for (auto b : balanceArray)
        {
            auto balanceObject = std::get<std::shared_ptr<utils::JSON::Object>>(b);
            ProductBalance balance;
            balance.account = magic_enum::enum_cast<BalanceAccount>(balanceObject->getStringValue("account")).value();
            balance.costs = balanceObject->getFloatValue("costs");
            balance.income = balanceObject->getFloatValue("income");
            balance.month = balanceObject->getIntValue("month");
            balance.year = balanceObject->getIntValue("year");
            balance.name = balanceObject->getStringValue("product");
            result->addBalance(balance);
        }

        utils::JSON::JsonArray componentsArray = object->getArray("components");
        for (auto &c : componentsArray)
        {
            auto componentObject = std::get<std::shared_ptr<utils::JSON::Object>>(c);
            auto componentName = componentObject->getStringValue("name");

            auto component = Building::createComponentByName(componentName)->clone();
            component->fromJson(componentObject, company);

            result->addComponent(component);
        }

        return result;
    }

    void Building::addComponent(std::shared_ptr<world::buildings::BuildingComponent> &component)
    {
        components[component->getName()] = component;
    }

    std::shared_ptr<world::buildings::BuildingComponent> Building::getComponentByName(const std::string &name)
    {
        return components.at(name);
    }

    bool Building::hasComponent(const std::string &name)
    {
        return components.count(name) > 0;
    }

    void Building::initComponentMap()
    {
        Building::componentMap["TransportComponent"] = std::make_shared<world::buildings::TransportComponent>();
        Building::componentMap["SalesComponent"] = std::make_shared<world::buildings::SalesComponent>();
        Building::componentMap["StorageComponent"] = std::make_shared<world::buildings::StorageComponent>();
        Building::componentMap["HouseComponent"] = std::make_shared<world::buildings::HouseComponent>();
        Building::componentMap["WorkerComponent"] = std::make_shared<world::buildings::WorkerComponent>();
    }

    std::shared_ptr<world::buildings::BuildingComponent> Building::createComponentByName(const std::string &name)
    {
        return Building::componentMap.at(name)->clone();
    }

    void Building::delayedUpdate(Company *company)
    {
        for (auto &component : components)
        {
            component.second->delayedUpdate(company);
        }
    }

    void Building::addBalance(ProductBalance value)
    {
        balance.push_back(value);
    }

    bool Building::requireResource(world::RawResource rawResource)
    {
        auto it = std::find(rawResources.begin(), rawResources.end(), rawResource);
        return it != rawResources.end();
    }
    void Building::addResource(world::RawResource rawResource)
    {
        rawResources.push_back(rawResource);
    }

}