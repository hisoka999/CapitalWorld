#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <map>

struct StorageEntry{
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

private:
    unsigned maximalAmount;

    std::map<std::string,StorageEntry> entries;
};

#endif // STORAGE_H
