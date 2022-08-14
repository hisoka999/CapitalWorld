#include "StorageComponent.h"

namespace world
{
    namespace buildings
    {
        StorageComponent::StorageComponent()
            : BuildingComponent("StorageComponent")
        {
            maximalAmount = 100000;
        }

        bool StorageComponent::canAdd([[maybe_unused]] const std::string &product, int amount)
        {
            int usage = static_cast<int>(usedStorage());
            return usage + amount < static_cast<int>(maximalAmount) && amount > 0;
        }
        void StorageComponent::addEntry(const std::string &product, int amount)
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
                if (int(entries[product].amount) - newAmount >= 0 || newAmount > 0)
                {
                    entries[product].amount += newAmount;
                }
                // count storage usage
            }
        }
        unsigned StorageComponent::usedStorage()
        {
            unsigned usage = 0;
            for (auto it = entries.begin(); it != entries.end(); it++)
            {
                usage += it->second.amount;
            }
            return usage;
        }

        std::vector<std::string> StorageComponent::getStoredProducts()
        {
            std::vector<std::string> result;
            for (auto it = entries.begin(); it != entries.end(); it++)
            {
                result.push_back(it->first);
            }
            return result;
        }

        StorageMap &StorageComponent::getEntries()
        {
            return entries;
        }

        std::shared_ptr<utils::JSON::Object> StorageComponent::toJson()
        {
            std::shared_ptr<utils::JSON::Object> json = BuildingComponent::toJson();

            for (auto &entry : entries)
            {
                json->setAttribute(entry.first, int(entry.second.amount));
            }
            return json;
        }

        void StorageComponent::fromJson(std::shared_ptr<utils::JSON::Object> &object, [[maybe_unused]] Company *company)
        {
            for (auto attr : object->getAttributes())
            {
                if (attr != "name")
                    addEntry(attr, object->getIntValue(attr));
            }
        }

        std::shared_ptr<BuildingComponent> StorageComponent::clone()
        {
            return std::make_shared<StorageComponent>();
        }

        int StorageComponent::getEntry(const std::string &product)
        {
            if (entries.count(product) == 0)
                return 0;
            return entries[product].amount;
        }

    } // namespace buildings

} // namespace world
