#include "gamemap.h"
#include <algorithm>
GameMap::GameMap(size_t width, size_t height) : width(width), height(height)
{
    initEmtyMap();
}

GameMap::GameMap(size_t width, size_t height, std::vector<TileType> mapData) : width(width), height(height), mapData(mapData)
{
    buildings.resize(width * height);
    std::fill(buildings.begin(), buildings.end(), nullptr);
}

void GameMap::initEmtyMap()
{
    mapData.clear();
    buildings.clear();
    mapData.resize(width * height);
    buildings.resize(width * height);
    std::fill(mapData.begin(), mapData.end(), 10);
    std::fill(buildings.begin(), buildings.end(), nullptr);
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
        if (building == nullptr)
            continue;
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
    int x = building->get2DPosition().x;
    int y = building->get2DPosition().y;
    size_t pos = x + (y * height);
    buildings[pos] = building;
    //buildings.push_back(building);
    building->update(this);
}
const std::vector<std::shared_ptr<world::Building>> &GameMap::getBuildings() const
{
    return buildings;
}

const std::shared_ptr<world::Building> &GameMap::getBuilding(const int x, const int y)
{
    size_t pos = x + (y * height);

    return buildings[pos];
}

const size_t GameMap::make_pos(const uint16_t x, const uint16_t y) const
{
    return x + (y * height);
}
bool GameMap::canBuild(graphics::Rect buildRect)
{
    if (buildRect.x < 0 || buildRect.y < 0)
        return false;

    for (auto &building : buildings)
    {
        if (building == nullptr)
            continue;
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

    return true;
}
void GameMap::removeBuilding(std::shared_ptr<world::Building> building)
{
    auto pos = building->get2DPosition();

    size_t posInMap = pos.x + (pos.y * height);
    buildings[posInMap] = nullptr;

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
        auto street = tmp;
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
std::vector<std::shared_ptr<world::Building>> GameMap::borderingBuilding(const std::shared_ptr<world::Building> &startBuilding, world::BuildingType buildingType, bool inverseType)
{

    auto pos = startBuilding->get2DPosition();

    std::vector<std::shared_ptr<world::Building>> result;

    auto northBuilding = getBuilding(pos.x, pos.y - 1);
    auto southBuilding = getBuilding(pos.x, pos.y + 1);
    auto eastBuilding = getBuilding(pos.x + pos.width, pos.y);
    auto westBuilding = getBuilding(pos.x - 1, pos.y);

    auto isType = [&](world::BuildingType type)
    {
        if (inverseType)
            return type != buildingType;

        return type == buildingType;
    };

    if (northBuilding != nullptr && isType(northBuilding->getType()))
    {
        result.push_back(northBuilding);
    }

    if (southBuilding != nullptr && isType(southBuilding->getType()))
    {
        result.push_back(southBuilding);
    }
    if (eastBuilding != nullptr && isType(eastBuilding->getType()))
    {
        result.push_back(eastBuilding);
    }
    if (westBuilding != nullptr && isType(westBuilding->getType()))
    {
        result.push_back(westBuilding);
    }
    return result;
}

void GameMap::findStreets(const std::shared_ptr<world::Building> &startBuilding, std::vector<std::shared_ptr<world::Building>> &streets, const std::shared_ptr<world::Building> &excludeStreet)
{

    for (auto street : borderingBuilding(startBuilding, world::BuildingType::Street, false))
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

std::vector<std::shared_ptr<world::Building>> GameMap::findProductionBuildings(const std::shared_ptr<world::Building> &startBuilding)
{
    //Schritt 1 Suche Straße neben dem Startgebäude
    auto startPos = startBuilding->get2DPosition();

    std::vector<std::shared_ptr<world::Building>> allStreets;
    // Schritt 2 suche alle Strassen zur Startstrasse
    for (auto street : borderingBuilding(startBuilding, world::BuildingType::Street, false))
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
            if (b != startBuilding && b->getProducts().size() > 0)
            {
                targets.push_back(b);
            }
        }
    }

    return targets;
}
