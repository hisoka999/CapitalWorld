#include "building.h"
#include <algorithm>

namespace world
{

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
            productBalance.income = 0; //TODO
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
                        int amount = storage.getEntry(base->product->getName());
                        if (amount < base->amount)
                        {
                            requirementsFullfilled = false;
                        }
                    }
                    if (requirementsFullfilled)
                    {
                        for (auto &base : product->getBaseProducts())
                        {
                            storage.addEntry(base->product->getName(), base->amount * -1);
                        }
                        storage.addEntry(product->getName(), cycle.amount);
                        updateProduction(month, year);
                    }
                }
                else if (product->getResources().size() > 0)
                {
                    storage.addEntry(product->getName(), cycle.amount);
                }
            }
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

    bool Building::isAutoSellActive()
    {
        return type == BuildingType::Factory;
    }

    void Building::autoSell(int month, int year)
    {
        for (auto &product : products)
        {
            unsigned amount = storage.getEntry(product->getName());
            double income = amount * product->calculateCostsPerPiece() * 1.5;

            //find balance
            for (auto &b : balance)
            {
                if (b.name == product->getName() && b.year == year && b.month == month && b.account == BalanceAccount::Production)
                {
                    b.income += income;
                }
            }
            storage.addEntry(product->getName(), amount * -1);
        }
    }
    Storage &Building::getStorage()
    {
        return storage;
    }
    const std::string &Building::getSubTexture()
    {
        return subTexture;
    }
    void Building::setSubTexture(const std::string &tex)
    {
        subTexture = tex;
    }

}
