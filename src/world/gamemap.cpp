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

const TileType GameMap::getTile(const size_t x, const size_t y) const
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
        if (building.second == nullptr)
            continue;
        if (building.second->get2DPosition().intersectsNoLine(sourceBuilding))
        {
            result = building.second;
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
    int x = building->get2DPosition().x;
    int y = building->get2DPosition().y;
    size_t pos = make_pos(x, y);
    buildings[pos] = building;
    //buildings.push_back(building);
    building->update(this);
}
const std::map<size_t, std::shared_ptr<world::Building>> &GameMap::getBuildings() const
{
    return buildings;
}

world::Building *GameMap::getBuilding(const int x, const int y)
{
    auto pos = make_pos(x, y);

    if (buildings.count(pos) > 0)
        return buildings[pos].get();
    return nullptr;
}

const size_t GameMap::make_pos(const uint16_t x, const uint16_t y) const
{
    return (y << 16) + x;
}
bool GameMap::canBuild(graphics::Rect buildRect)
{
    if (buildRect.x < 0 || buildRect.y < 0)
        return false;

    for (auto &building : buildings)
    {
        if (building.second == nullptr)
            continue;
        if (building.second->get2DPosition().intersectsNoLine(buildRect))
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

    return true;
}
void GameMap::removeBuilding(std::shared_ptr<world::Building> building)
{
    auto pos = building->get2DPosition();

    const size_t posInMap = make_pos(uint16_t(pos.x), uint16_t(pos.y));
    buildings.erase(posInMap);

    auto posNorth = pos;
    posNorth.y -= 1;
    auto posEast = pos;
    posEast.x += 1;
    auto posWest = pos;
    posWest.x -= 1;
    auto posSouth = pos;
    posSouth.y += 1;

    //find parent buildings
    for (auto &tmp : buildings)
    {
        auto street = tmp.second;
        if (street->getType() != world::BuildingType::Street)
            continue;

        if (street->get2DPosition().intersectsNoLine(posNorth))
        {
            street->update(this);
        }
        if (street->get2DPosition().intersectsNoLine(posSouth))
        {
            street->update(this);
        }
        if (street->get2DPosition().intersectsNoLine(posEast))
        {
            street->update(this);
        }
        if (street->get2DPosition().intersectsNoLine(posWest))
        {
            street->update(this);
        }
    }
}
std::vector<std::shared_ptr<world::Building>> GameMap::borderingBuilding(std::shared_ptr<world::Building> &startBuilding, world::BuildingType buildingType, bool inverseType)
{

    auto pos = startBuilding->get2DPosition();

    std::vector<std::shared_ptr<world::Building>> result;

    auto northBuilding = getBuilding(pos.x, pos.y - 1);
    auto southBuilding = getBuilding(pos.x, pos.y + 1);
    auto eastBuilding = getBuilding(pos.x + 1, pos.y);
    auto westBuilding = getBuilding(pos.x - 1, pos.y);

    if (northBuilding != nullptr && northBuilding->getType() == buildingType)
    {
        result.push_back(std::shared_ptr<world::Building>(northBuilding));
    }

    if (southBuilding != nullptr && southBuilding->getType() == buildingType)
    {
        result.push_back(std::shared_ptr<world::Building>(southBuilding));
    }
    if (eastBuilding != nullptr && eastBuilding->getType() == buildingType)
    {
        result.push_back(std::shared_ptr<world::Building>(eastBuilding));
    }
    if (westBuilding != nullptr && westBuilding->getType() == buildingType)
    {
        result.push_back(std::shared_ptr<world::Building>(westBuilding));
    }
    return result;
}

void GameMap::findStreets(std::shared_ptr<world::Building> &startBuilding, std::vector<std::shared_ptr<world::Building>> &streets, std::shared_ptr<world::Building> &excludeStreet)
{
    auto borderingStreets = borderingBuilding(startBuilding, world::BuildingType::Street, false);

    for (auto &street : borderingStreets)
    {
        if (excludeStreet != nullptr && excludeStreet == street)
            continue;

        auto it = std::find(streets.begin(), streets.end(), street);

        if (it == std::end(streets))
        {
            streets.push_back(street);
            findStreets(street, streets, startBuilding);
        }
    }
}

std::vector<std::shared_ptr<world::Building>> GameMap::findProductionBuildings(std::shared_ptr<world::Building> &startBuilding)
{
    //Schritt 1 Suche Straße neben dem Startgebäude
    auto startPos = startBuilding->get2DPosition();
    auto streets = borderingBuilding(startBuilding, world::BuildingType::Street, false);

    std::vector<std::shared_ptr<world::Building>> allStreets;
    // Schritt 2 suche alle Strassen zur Startstrasse
    for (auto &street : streets)
    {
        allStreets.push_back(street);
        findStreets(street, allStreets, street);
    }

    // Schritt 3 suche alle gebaeude an den Strassen
    std::vector<std::shared_ptr<world::Building>> targets;
    for (auto &street : allStreets)
    {
        //std::cout << "steet x: " << street->get2DPosition().x << " y: " << street->get2DPosition().y << std::endl;

        auto borderBuildings = borderingBuilding(street, world::BuildingType::Street, true);
        for (auto &b : borderBuildings)
        {
            //std::cout << "b x: " << b->get2DPosition().x << " y: " << b->get2DPosition().y << std::endl;
            if (b != startBuilding)
            {
                targets.push_back(b);
            }
        }
    }

    return targets;
}
