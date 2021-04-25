#include "gamemap.h"
#include <algorithm>
GameMap::GameMap(size_t width, size_t height) : width(width), height(height)
{
    initEmtyMap();
}

GameMap::GameMap(size_t width, size_t height, std::vector<TileType> mapData) : width(width), height(height), mapData(mapData)
{
}

void GameMap::initEmtyMap()
{
    mapData.clear();
    mapData.resize(width * height);
    std::fill(mapData.begin(), mapData.end(), 10);
}

TileType GameMap::getTile(size_t x, size_t y)
{
    size_t pos = x + (y * height);
    if (pos > mapData.size())
        return 0;
    return mapData[pos];
}

TileType GameMap::getTile(utils::Vector2 &pos)
{
    return getTile(pos.getX(), pos.getY());
}

size_t GameMap::getWidth()
{
    return width;
}
size_t GameMap::getHeight()
{
    return height;
}

utils::Vector2 GameMap::isoTo2D(utils::Vector2 pt)
{
    float x, y = 0.0f;
    x = ((2.0f * pt.getY()) + pt.getX()) / 2.0f;
    y = ((2.0f * pt.getY()) - pt.getX()) / 2.0f;
    return utils::Vector2(x, y);
}

utils::Vector2 GameMap::twoDToIso(utils::Vector2 pt)
{
    float x, y = 0.0f;
    x = pt.getX() - pt.getY();
    y = (pt.getX() + pt.getY()) / 2.0f;
    return utils::Vector2(x, y);
}
std::shared_ptr<world::Building> GameMap::getBuilding2D(const graphics::Rect &sourceBuilding)
{
    std::shared_ptr<world::Building> result = nullptr;
    //auto iso = twoDToIso(pt);
    for (auto building : buildings)
    {
        if (building->get2DPosition().intersectsNoLine(sourceBuilding))
        {
            result = building;
            break;
        }
    }

    return result;
}
bool compareBuilding(std::shared_ptr<world::Building> b1, std::shared_ptr<world::Building> b2)
{
    return b1->getDisplayRect().y < b2->getDisplayRect().y;
}

void GameMap::addBuilding(std::shared_ptr<world::Building> building)
{
    buildings.push_back(building);
    std::sort(buildings.begin(), buildings.end(), compareBuilding);
}
std::vector<std::shared_ptr<world::Building>> GameMap::getBuildings()
{
    return buildings;
}
bool GameMap::canBuild(graphics::Rect buildRect)
{
    if (buildRect.x < 0 || buildRect.y < 0)
        return false;

    for (auto building : buildings)
    {
        if (building->get2DPosition().intersectsNoLine(buildRect))
            return false;
    }

    TileType tile = getTile(buildRect.x, buildRect.y);

    size_t tileX = buildRect.x;
    size_t tileY = buildRect.y;
    int groundLimit = 10;

    if (tile < 8)
    {
        //render water
        return false;
    }
    else if (tile > groundLimit)
    {
        //render mountain
        return true;
    }
    else
    {
        //ground

        if (getTile(tileX - 1, tileY) > groundLimit &&
            getTile(tileX, tileY - 1) > groundLimit)
        {
            return false;
        }
        else if (getTile(tileX - 1, tileY) > groundLimit &&
                 getTile(tileX, tileY + 1) > groundLimit)
        {
            return false;
        }
        else if (getTile(tileX, tileY - 1) > groundLimit)
        {
            return false;
        }
        else if (getTile(tileX, tileY + 1) > groundLimit)
        {
            return false;
        }
        else if (getTile(tileX - 1, tileY) > groundLimit)
        {
            return false;
        }
        else if (getTile(tileX + 1, tileY) > groundLimit)
        {
            return false;
        }
        else if (getTile(tileX - 1, tileY - 1) > groundLimit)
        {
            return false;
        }
        else if (getTile(tileX - 1, tileY + 1) > groundLimit)
        {
            return false;
        }
        else if (getTile(tileX + 1, tileY + 1) > groundLimit)
        {
            return false;
        }
        else if (getTile(tileX + 1, tileY - 1) > groundLimit)
        {
            return false;
        }
    }

    return true;
}
void GameMap::removeBuilding(std::shared_ptr<world::Building> building)
{
    auto it = std::find(buildings.begin(), buildings.end(), building);
    if (it != buildings.end())
    {
        buildings.erase(it);
    }
}
