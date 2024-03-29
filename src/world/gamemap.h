#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "building.h"
#include <cstdint>
#include <engine/utils/vector2.h>
#include <map>
#include <memory>
#include <vector>
#include <world/company.h>
#include <engine/paths/dijkstra.h>

typedef uint16_t TileType;

enum class Decoration : TileType
{
    none = 0,
    grass1 = 1,
    rocks = 2,
    trees = 3,
    mountain = 4,
};

std::string tileTypeToString(const TileType tile);

class GameMap
{
public:
    GameMap(size_t width, size_t height);
    GameMap(size_t width, size_t height, std::vector<TileType> mapData, std::vector<TileType> mapDecoration, std::vector<world::RawResource> mapResources);
    virtual ~GameMap();
    GameMap(const GameMap &copy);

    void initEmtyMap();

    TileType getTile(const int x, const int y) const;
    TileType getTile(const utils::Vector2 &pos);
    TileType getDecoration(const utils::Vector2 &pos);
    TileType getDecoration(const int x, const int y) const;
    world::RawResource getRawResource(const int x, const int y) const;

    size_t getWidth() const;
    size_t getHeight() const;

    std::shared_ptr<world::Building> getBuilding2D(const graphics::Rect &sourceBuilding);
    bool canBuild(graphics::Rect buildRect);
    void addBuilding(std::shared_ptr<world::Building> building);
    void removeBuilding(std::shared_ptr<world::Building> building);
    const std::vector<std::shared_ptr<world::Building>> &getBuildings() const;
    const std::shared_ptr<world::Building> &getBuilding(const int x, const int y) const;
    std::vector<std::shared_ptr<world::Building>> findStorageBuildings(const std::shared_ptr<world::Building> &startBuilding, const std::shared_ptr<world::Company> &company);
    std::vector<std::shared_ptr<world::Building>> findByComponentTypeInDistance(const std::string &componentType, world::Building *startBuilding, const int distance);

    std::shared_ptr<utils::JSON::Object> toJson();
    static std::shared_ptr<GameMap> fromJson(const std::shared_ptr<utils::JSON::Object> &object);
    std::vector<utils::Vector2> &getChangedTiles();
    void clearChangedTiles();
    paths::Graph &getStreetGraph();
    size_t make_pos(const utils::Vector2 &pos);
    size_t getGraphIndex(const utils::Vector2 &pos);
    std::vector<std::shared_ptr<world::Building>> borderingBuilding(const std::shared_ptr<world::Building> &startBuilding, world::BuildingType buildingType, bool inverseType);

private:
    size_t make_pos(const uint16_t x, const uint16_t y) const;
    void findStreets(const std::shared_ptr<world::Building> &startBuilding, std::vector<std::shared_ptr<world::Building>> &streets, const std::shared_ptr<world::Building> &excludeStreet);
    void initStreetGraph();
    void initParents(const std::shared_ptr<world::Building> &building);
    size_t width, height;
    TileType *mapData;
    std::vector<TileType> mapDecoration;
    std::vector<world::RawResource> mapResources;
    std::vector<std::shared_ptr<world::Building>> buildings;
    std::vector<utils::Vector2> changedTiles;
    paths::Graph streetGraph;
    std::map<size_t, size_t> graphIndices;
};

#endif // GAMEMAP_H
