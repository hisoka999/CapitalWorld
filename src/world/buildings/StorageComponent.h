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
            unsigned amount;
        };

        class StorageComponent : public world::buildings::BuildingComponent
        {
        public:
            StorageComponent();
            bool canAdd(const std::string &product, int amount);
            void addEntry(const std::string &product, int amount);
            unsigned getEntry(std::string product);
            unsigned usedStorage();
            std::vector<std::string> getStoredProducts();

            virtual std::shared_ptr<utils::JSON::Object> toJson();
            virtual void fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company);
            virtual std::shared_ptr<BuildingComponent> clone();

        private:
            unsigned maximalAmount;

            std::map<std::string, StorageEntry> entries;
        };
    } // namespace buildings

} // namespace world

#endif // __STORAGECOMPONENT_H__