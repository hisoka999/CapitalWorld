#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <map>
#include <vector>
#include <engine/utils/json/object.h>

struct StorageEntry
{
    std::string product;
    unsigned amount;
};

class Storage
{
public:
    Storage();

    void addEntry(std::string product, int amount);
    unsigned getEntry(std::string product);
    unsigned usedStorage();
    std::vector<std::string> getStoredProducts();

    std::shared_ptr<utils::JSON::Object> toJson();

private:
    unsigned maximalAmount;

    std::map<std::string, StorageEntry> entries;
};

#endif // STORAGE_H
