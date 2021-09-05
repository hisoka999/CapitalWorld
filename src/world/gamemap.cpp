#include "gamemap.h"
#include <algorithm>
#include "translate.h"

bool isBuildingSmaller(std::shared_ptr<world::Building> &b1, std::shared_ptr<world::Building> &b2)
{
    return b1->getDisplayName() < b2->getDisplayName();
}
GameMap::GameMap(size_t width, size_t height) : width(width), height(height)
{
    initEmtyMap();
}

GameMap::GameMap(size_t width, size_t height, std::vector<TileType> mapData, std::vector<TileType> mapDecoration) : width(width), height(height), mapData(mapData), mapDecoration(mapDecoration)
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
    mapDecoration.resize(width * height);
    std::fill(mapData.begin(), mapData.end(), 10);
    std::fill(buildings.begin(), buildings.end(), nullptr);
    std::fill(mapDecoration.begin(), mapDecoration.end(), 0);
}

const TileType GameMap::getTile(const int x, const int y) const
{
    int pos = x + (y * height);
    if (pos > mapData.size())
        return 0;
    else if (x < 0 || y < 0)
        return 0;
    return mapData[pos];
}

TileType GameMap::getTile(utils::Vector2 &pos)
{
    return getTile(pos.getX(), pos.getY());
}

TileType GameMap::getDecoration(utils::Vector2 &pos)
{
    return getDecoration(pos.getX(), pos.getY());
}

const TileType GameMap::getDecoration(const int x, const int y) const
{
    int pos = x + (y * height);
    if (pos > mapDecoration.size())
        return 0;
    else if (x < 0 || y < 0)
        return 0;
    return mapDecoration[pos];
}

const size_t GameMap::getWidth() const
{
    return width;
}
const size_t GameMap::getHeight() const
{
    return height;
}

std::shared_ptr<world::Building> GameMap::getBuilding2D(const graphics::Rect &sourceBuilding)
{
    std::shared_ptr<world::Building> result = nullptr;
    //auto iso = twoDToIso(pt);
    for (int y = sourceBuilding.y - sourceBuilding.height; y <= sourceBuilding.y + sourceBuilding.height; ++y)
    {
        for (int x = sourceBuilding.x - sourceBuilding.width; x <= sourceBuilding.x + sourceBuilding.width; ++x)
        {
            auto &building = getBuilding(x, y);
            if (building == nullptr)
                continue;
            if (building->get2DPosition().intersectsNoLine(sourceBuilding))
            {
                result = building;
                break;
            }
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

const std::shared_ptr<world::Building> &GameMap::getBuilding(const int x, const int y) const
{
    int x2 = x;
    int y2 = y;
    if (x2 < 0)
        x2 = 0;
    if (y2 < 0)
        y2 = 0;
    size_t pos = x2 + (y2 * height);
    if (pos > buildings.size())
        pos = buildings.size() - 1;

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

    for (int y = buildRect.y - buildRect.height; y <= buildRect.y + buildRect.height; ++y)
    {
        for (int x = buildRect.x - buildRect.width; x <= buildRect.x + buildRect.width; ++x)
        {
            const std::shared_ptr<world::Building> &building = getBuilding(x, y);
            if (building == nullptr)
                continue;
            if (building->get2DPosition().intersectsNoLine(buildRect))
                return false;
        }
    }

    TileType tile = getTile(buildRect.x, buildRect.y);

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
        if (tmp == nullptr)
            continue;
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

    auto northBuilding = getBuilding2D({pos.x, pos.y - 1, pos.width, pos.height});
    auto southBuilding = getBuilding2D({pos.x, pos.y + pos.height, pos.width, pos.height});
    auto eastBuilding = getBuilding2D({pos.x + pos.width, pos.y, pos.width, pos.height});
    auto westBuilding = getBuilding2D({pos.x - 1, pos.y, pos.width, pos.height});

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

std::vector<std::shared_ptr<world::Building>> GameMap::findStorageBuildings(const std::shared_ptr<world::Building> &startBuilding, const std::shared_ptr<world::Company> &company)
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
            if (b != startBuilding && (b->hasComponent("StorageComponent")) && company->hasBuilding(b))
            {
                auto it = std::find_if(targets.begin(), targets.end(), [=](std::shared_ptr<world::Building> &b2)
                                       { return b->getDisplayName() == b2->getDisplayName(); });
                if (it == std::end(targets))
                {
                    targets.push_back(b);
                }
            }
        }
    }
    std::sort(targets.begin(), targets.end(), isBuildingSmaller);

    return targets;
}

std::vector<std::shared_ptr<world::Building>> GameMap::findHousesInDistance(world::Building *startBuilding, const int distance)
{
    std::vector<std::shared_ptr<world::Building>> result;
    auto startRect = startBuilding->get2DPosition();
    utils::Vector2 startPos(startRect.x, startRect.y);
    for (auto building : buildings)
    {
        if (building == nullptr)
            continue;

        if (building->hasComponent("HouseComponent"))
        {
            auto rect = building->get2DPosition();
            utils::Vector2 endPos(rect.x, rect.y);
            if (endPos.distance(startPos) < distance)
            {
                result.push_back(building);
            }
        }
    }
    return result;
}

std::shared_ptr<utils::JSON::Object> GameMap::toJson()
{
    std::shared_ptr<utils::JSON::Object> json = std::make_shared<utils::JSON::Object>();

    std::string tileData = "";
    for (TileType tile : mapData)
    {
        tileData += ('0' + tile);
    }
    std::string decoration = "";
    for (TileType tile : mapDecoration)
    {
        decoration += ('0' + tile);
    }
    json->setAttribute("width", int(getWidth()));
    json->setAttribute("height", int(getHeight()));
    json->setAttribute("mapData", tileData);
    json->setAttribute("mapDecoraction", decoration);

    return json;
}

std::shared_ptr<GameMap> GameMap::fromJson(const std::shared_ptr<utils::JSON::Object> &object)
{
    int width = object->getIntValue("width");
    int height = object->getIntValue("height");
    std::string mapData = object->getStringValue("mapData");
    std::string mapDecoraction = object->getStringValue("mapDecoraction");

    std::vector<TileType> tiles;
    std::vector<TileType> decoration;
    //tiles.reserve(width * height);
    //std::fill(tiles.begin(), tiles.end(), 10);

    for (char val : mapData)
    {
        TileType tile = static_cast<TileType>(val - '0');
        //tiles[i] = tile;
        tiles.push_back(tile);
    }

    for (char val : mapDecoraction)
    {
        TileType tile = static_cast<TileType>(val - '0');
        //tiles[i] = tile;
        decoration.push_back(tile);
    }

    return std::make_shared<GameMap>(width, height, tiles, decoration);
}

std::string tileTypeToString(const TileType tile)
{
    const int groundLimit = 8;

    if (tile < groundLimit)
    {
        return _("Water");
    }
    else if (tile > 12)
    {
        return _("Trees");
    }
    else if (groundLimit == tile)
    {
        return _("Sand");
    }

    return _("Grass");
}
