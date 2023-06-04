#ifndef __STORAGECOMPONENT_H__
#define __STORAGECOMPONENT_H__

#include <string>
#include <map>
#include <vector>
#include <engine/utils/json/object.h>
#include <world/buildings/BuildingComponent.h>

namespace world
{
    namespace buildings
    {

        struct StorageEntry
        {
            std::string product;
            int amount;
        };
        typedef std::map<std::string, StorageEntry> StorageMap;
        typedef StorageMap::iterator StorageMapIterator;
        class StorageComponent : public world::buildings::BuildingComponent
        {
        public:
            StorageComponent();
            bool canAdd(const std::string &product, int amount);
            void addEntry(const std::string &product, int amount);
            int getEntry(const std::string &product);
            unsigned usedStorage(const std::string &product);
            std::vector<std::string> getStoredProducts();
            StorageMap &getEntries();

            virtual std::shared_ptr<utils::JSON::Object> toJson();
            virtual void fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company);
            virtual std::shared_ptr<BuildingComponent> clone();

        private:
            unsigned maximalAmount;

            StorageMap entries;
        };
    } // namespace buildings

} // namespace world

#endif // __STORAGECOMPONENT_H__