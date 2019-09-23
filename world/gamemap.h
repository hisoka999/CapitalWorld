#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>
#include <cstdint>
#include <engine/utils/vector2.h>
#include <world/building.h>
#include <memory>

class GameMap
{
public:
    GameMap(size_t width,size_t height);
    void initEmtryMap();

    uint16_t getTile(int x,int y);
    uint16_t getTile(utils::Vector2& pos);

    size_t getWidth();
    size_t getHeight();


    utils::Vector2 isoTo2D(utils::Vector2 pt);
    utils::Vector2 twoDToIso(utils::Vector2 pt);

    std::shared_ptr<world::Building> getBuilding2D(utils::Vector2 pt);
    bool canBuild(graphics::Rect buildRect);
    void addBuilding(std::shared_ptr<world::Building> building);
    std::vector<std::shared_ptr<world::Building>> getBuildings();

private:
    size_t width,height;
    std::vector<uint16_t> mapData;
    std::vector<std::shared_ptr<world::Building>> buildings;
};

#endif // GAMEMAP_H
