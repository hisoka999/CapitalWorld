#include "gamemap.h"
#include "translate.h"
#include <algorithm>
#include "messages.h"
#include <mutex>

std::mutex gGameMapMutex;

bool isBuildingSmaller(std::shared_ptr<world::Building> &b1, std::shared_ptr<world::Building> &b2)
{
    return b1->getDisplayName() < b2->getDisplayName();
}
GameMap::GameMap(size_t width, size_t height) : width(width), height(height), mapData(nullptr)
{
    initEmtyMap();
}

GameMap::GameMap(size_t width, size_t height, std::vector<TileType> mapData, std::vector<TileType> mapDecoration, std::vector<world::RawResource> mapResources) : width(width), height(height), mapDecoration(mapDecoration), mapResources(mapResources)
{
    buildings.resize(width * height);
    std::fill(buildings.begin(), buildings.end(), nullptr);
    this->mapData = nullptr;
    this->mapData = new TileType[width * height];
    for (size_t i = 0; i < mapData.size(); ++i)
        this->mapData[i] = mapData[i];
}

GameMap::~GameMap()
{
    buildings.clear();
    mapDecoration.clear();
    buildings.resize(0);
    mapDecoration.resize(0);
    delete[] mapData;
}

GameMap::GameMap(const GameMap &copy)
{
    width = copy.width;
    height = copy.height;
}

void GameMap::initEmtyMap()
{
    if (mapData != nullptr)
    {
        delete[] mapData;
        this->mapData = nullptr;
        this->mapData = new TileType[width * height];
        for (size_t i = 0; i < width * height; ++i)
            mapData[i] = 10;
    }
    buildings.clear();
    // mapData.resize(width * height);
    buildings.resize(width * height);
    mapDecoration.resize(width * height);
    // std::fill(mapData.begin(), mapData.end(), 10);
    std::fill(buildings.begin(), buildings.end(), nullptr);
    std::fill(mapDecoration.begin(), mapDecoration.end(), 0);
}

TileType GameMap::getTile(const int x, const int y) const
{
    int pos = x + (y * height);
    if (pos > int(width * height))
        return 0;
    else if (x < 0 || y < 0)
        return 0;
    return mapData[pos];
}

TileType GameMap::getTile(const utils::Vector2 &pos)
{
    return getTile(pos.getX(), pos.getY());
}

TileType GameMap::getDecoration(const utils::Vector2 &pos)
{
    return getDecoration(pos.getX(), pos.getY());
}

TileType GameMap::getDecoration(const int x, const int y) const
{
    int pos = x + (y * height);

    if (x < 0 || y < 0)
        return 0;
    else if (pos > int(mapDecoration.size()))
        return 0;
    return mapDecoration[pos];
}

world::RawResource GameMap::getRawResource(const int x, const int y) const
{
    int pos = x + (y * height);

    if (x < 0 || y < 0)
        return world::RawResource::None;
    else if (pos > int(mapResources.size()))
        return world::RawResource::None;
    return mapResources[pos];
}

size_t GameMap::getWidth() const
{
    return width;
}
size_t GameMap::getHeight() const
{
    return height;
}

std::shared_ptr<world::Building> GameMap::getBuilding2D(const graphics::Rect &sourceBuilding)
{
    std::shared_ptr<world::Building> result = nullptr;
    // auto iso = twoDToIso(pt);
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
    std::lock_guard<std::mutex> guard(gGameMapMutex);

    auto position = building->get2DPosition();
    for (int x = position.x; x < position.x + position.width; ++x)
    {
        for (int y = position.y; y < position.y + position.height; ++y)
        {
            size_t pos = x + (y * height);
            buildings[pos] = building;

            if (getTile(x, y) > 12)
            {
                mapData[pos] = 12;
                changedTiles.push_back({float(x), float(y)});
            }
        }
    }
    building->update(this);
    std::shared_ptr<core::Message<MessageTypes, bool>> message = std::make_shared<core::Message<MessageTypes, bool>>(MessageTypes::ObjectHasBuild, true);
    core::MessageSystem<MessageTypes>::get().sendMessage(message);
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

size_t GameMap::make_pos(const uint16_t x, const uint16_t y) const
{
    return x + (y * height);
}
bool GameMap::canBuild(graphics::Rect buildRect)
{
    if (buildRect.x < 0 || buildRect.y < 0)
        return false;

    if (make_pos(buildRect.x, buildRect.y) > buildings.size())
        return false;

    for (int y = buildRect.y - buildRect.height; y <= buildRect.y + buildRect.height; ++y)
    {
        for (int x = buildRect.x - buildRect.width; x <= buildRect.x + buildRect.width; ++x)
        {
            if (x + (y * height) >= width * height)
                return false;

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
        // render water
        return false;
    }

    return true;
}
void GameMap::removeBuilding(std::shared_ptr<world::Building> building)
{
    auto pos = building->get2DPosition();

    for (int x = pos.x; x < pos.x + pos.width; ++x)
    {
        for (int y = pos.y; y < pos.y + pos.height; ++y)
        {
            size_t pos = x + (y * height);
            buildings[pos] = nullptr;
        }
    }

    auto posNorth = pos;
    posNorth.y -= 1;
    auto posEast = pos;
    posEast.x += 1;
    auto posWest = pos;
    posWest.x -= 1;
    auto posSouth = pos;
    posSouth.y += 1;

    // find parent buildings
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

    auto northBuilding = getBuilding2D({pos.x, pos.y - 1, pos.width, 1});
    auto southBuilding = getBuilding2D({pos.x, pos.y + pos.height, 1, pos.height});
    auto eastBuilding = getBuilding2D({pos.x + pos.width, pos.y, pos.width, 1});
    auto westBuilding = getBuilding2D({pos.x - 1, pos.y, 1, pos.height});

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
    // Schritt 1 Suche Straße neben dem Startgebäude
    // auto startPos = startBuilding->get2DPosition();

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

        auto borderBuildings = borderingBuilding(street, world::BuildingType::Street, true);
        for (auto &b : borderBuildings)
        {
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

std::vector<std::shared_ptr<world::Building>> GameMap::findByComponentTypeInDistance(const std::string &componentType, world::Building *startBuilding, const int distance)
{
    std::vector<std::shared_ptr<world::Building>> result;
    auto startRect = startBuilding->get2DPosition();
    utils::Vector2 startPos(startRect.x, startRect.y);

    for (int y = startRect.y - distance; y <= startRect.y + distance; ++y)
    {
        for (int x = startRect.x - distance; x <= startRect.x + distance; ++x)
        {
            auto &building = getBuilding(x, y);
            if (building == nullptr || building->getType() == world::BuildingType::Street)
                continue;

            if (building->hasComponent(componentType))
            {
                if (std::find(result.begin(), result.end(), building) == std::end(result))
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
    for (size_t i = 0; i < width * height; ++i)
    {
        TileType tile = mapData[i];
        tileData += ('0' + tile);
    }
    std::string decoration = "";
    for (TileType tile : mapDecoration)
    {
        decoration += ('0' + tile);
    }
    std::string resources = "";
    for (world::RawResource res : mapResources)
    {
        resources += ('0' + (int)res);
    }
    json->setAttribute("width", int(getWidth()));
    json->setAttribute("height", int(getHeight()));
    json->setAttribute("mapData", tileData);
    json->setAttribute("mapDecoraction", decoration);
    json->setAttribute("mapResources", resources);

    return json;
}

std::shared_ptr<GameMap> GameMap::fromJson(const std::shared_ptr<utils::JSON::Object> &object)
{
    int width = object->getIntValue("width");
    int height = object->getIntValue("height");
    std::string mapData = object->getStringValue("mapData");
    std::string mapDecoraction = object->getStringValue("mapDecoraction");

    std::string mapResources = object->getStringValue("mapResources");

    std::vector<TileType> tiles;
    std::vector<TileType> decoration;
    std::vector<world::RawResource> resources;
    // tiles.reserve(width * height);
    // std::fill(tiles.begin(), tiles.end(), 10);

    for (char val : mapData)
    {
        TileType tile = static_cast<TileType>(val - '0');
        // tiles[i] = tile;
        tiles.push_back(tile);
    }

    for (char val : mapDecoraction)
    {
        TileType tile = static_cast<TileType>(val - '0');
        // tiles[i] = tile;
        decoration.push_back(tile);
    }
    for (char val : mapResources)
    {
        world::RawResource tile = static_cast<world::RawResource>(val - '0');
        // tiles[i] = tile;
        resources.push_back(tile);
    }

    return std::make_shared<GameMap>(width, height, tiles, decoration, resources);
}

std::vector<utils::Vector2> &GameMap::getChangedTiles()
{
    return changedTiles;
}

void GameMap::clearChangedTiles()
{
    changedTiles.clear();
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
