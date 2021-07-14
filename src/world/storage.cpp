#include "storage.h"

Storage::Storage()
{
    maximalAmount = 10000;
}
void Storage::addEntry(std::string product, int amount)
{
    int usage = static_cast<int>(usedStorage());
    int newAmount = amount;
    if (usage + amount > static_cast<int>(maximalAmount))
    {
        newAmount = static_cast<int>(maximalAmount) - usage;
    }

    if (entries.count(product) == 0 && newAmount > 0)
    {
        StorageEntry entry;
        entry.amount = static_cast<unsigned>(newAmount);
        entry.product = product;
        entries[product] = entry;
    }
    else
    {
        if (entries[product].amount - newAmount >= 0)
        {
            entries[product].amount += newAmount;
        }
        //count storage usage
    }
}
unsigned Storage::usedStorage()
{
    unsigned usage = 0;
    for (auto it = entries.begin(); it != entries.end(); it++)
    {
        usage += it->second.amount;
    }
    return usage;
}

std::vector<std::string> Storage::getStoredProducts()
{
    std::vector<std::string> result;
    for (auto it = entries.begin(); it != entries.end(); it++)
    {
        result.push_back(it->first);
    }
    return result;
}

std::shared_ptr<utils::JSON::Object> Storage::toJson()
{
    std::shared_ptr<utils::JSON::Object> json = std::make_shared<utils::JSON::Object>();

    for (auto &entry : entries)
    {
        json->setAttribute(entry.first, int(entry.second.amount));
    }
    return json;
}

unsigned Storage::getEntry(std::string product)
{
    return entries[product].amount;
}
