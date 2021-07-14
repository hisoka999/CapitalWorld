#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>
#include <map>
#include <cstdint>
#include <engine/utils/vector2.h>
#include "building.h"
#include <memory>

typedef uint16_t TileType;

class GameMap
{
public:
    GameMap(size_t width, size_t height);
    GameMap(size_t width, size_t height, std::vector<TileType> mapData);

    void initEmtyMap();

    const TileType getTile(const size_t x, const size_t y) const;
    TileType getTile(utils::Vector2 &pos);

    const size_t getWidth() const;
    const size_t getHeight() const;

    const utils::Vector2 isoTo2D(const utils::Vector2 &pt);
    const utils::Vector2 twoDToIso(const utils::Vector2 &pt);

    std::shared_ptr<world::Building> getBuilding2D(const graphics::Rect &sourceBuilding);
    bool canBuild(graphics::Rect buildRect);
    void addBuilding(std::shared_ptr<world::Building> building);
    void removeBuilding(std::shared_ptr<world::Building> building);
    const std::vector<std::shared_ptr<world::Building>> &getBuildings() const;
    const std::shared_ptr<world::Building> &getBuilding(const int x, const int y) const;
    std::vector<std::shared_ptr<world::Building>> findProductionBuildings(const std::shared_ptr<world::Building> &startBuilding);

    std::shared_ptr<utils::JSON::Object> toJson();
    static std::shared_ptr<GameMap> fromJson(const std::shared_ptr<utils::JSON::Object> &object);

private:
    const size_t make_pos(const uint16_t x, const uint16_t y) const;
    std::vector<std::shared_ptr<world::Building>> borderingBuilding(const std::shared_ptr<world::Building> &startBuilding, world::BuildingType buildingType, bool inverseType);
    void findStreets(const std::shared_ptr<world::Building> &startBuilding, std::vector<std::shared_ptr<world::Building>> &streets, const std::shared_ptr<world::Building> &excludeStreet);

    size_t width, height;
    std::vector<TileType> mapData;
    std::vector<std::shared_ptr<world::Building>> buildings;
};

#endif // GAMEMAP_H
