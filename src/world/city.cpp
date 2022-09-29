#include "city.h"

#include "../translate.h"
#include "iso.h"
#include "world/buildings/HouseComponent.h"
#include <algorithm>
#include <cmath>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/color.h>
#include <engine/utils/os.h>
#include <random>
#include <engine/utils/logger.h>
#include <cmath>

namespace world
{

    City::City(std::string name, utils::Vector2 &position)
        : position(position), name(name), numberOfCitizen(0)
    {
        groundTexture = graphics::TextureManager::Instance().loadTextureMap(utils::os::combine("images", "tiles", "iso_tiles.json"));
        font = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 20);
    }

    void City::addCitizen(int people)
    {
        numberOfCitizen += people;
    }

    bool City::isBlocked(graphics::Rect rect, const std::shared_ptr<GameMap> &gameMap)
    {
        bool blocked = false;

        for (auto &street : streets)
        {
            if (street->get2DPosition().intersectsNoLine(rect))
            {
                blocked = true;
                break;
            }
        }
        if (!blocked)
        {
            for (auto &building : buildings)
            {
                if (building->get2DPosition().intersectsNoLine(rect))
                {
                    blocked = true;
                    break;
                }
            }
        }
        if (!blocked && !gameMap->canBuild(rect))
        {
            blocked = true;
        }

        return blocked;
    }
    bool City::isBorderingStreet(std::shared_ptr<Building> &building)
    {
        bool isBordering = false;
        auto pos = building->get2DPosition();
        auto posNorth = pos;
        posNorth.y -= pos.height;
        auto posEast = pos;
        posEast.x -= 1;
        auto posWest = pos;
        posWest.x += pos.width;
        auto posSouth = pos;
        posSouth.y += 1;
        for (auto &street : streets)
        {
            if (street->get2DPosition().intersectsNoLine(posNorth) || street->get2DPosition().intersectsNoLine(posEast) || street->get2DPosition().intersectsNoLine(posWest) || street->get2DPosition().intersectsNoLine(posSouth))
            {
                isBordering = true;
                break;
            }
        }

        return isBordering;
    }

    bool City::isOverlapStreet(std::shared_ptr<Building> &building)
    {
        bool isBordering = false;
        auto pos = building->get2DPosition();
        auto posNorth = pos;
        posNorth.y -= 1;
        posNorth.x -= 1;
        auto posEast = pos;
        posEast.x -= 1;
        posNorth.y += 1;
        auto posWest = pos;
        posWest.x += 1;
        posNorth.y -= 1;
        auto posSouth = pos;
        posSouth.y += 1;
        posNorth.x += 1;
        for (auto &street : streets)
        {
            if (street->get2DPosition().intersectsNoLine(posNorth) || street->get2DPosition().intersectsNoLine(posEast) || street->get2DPosition().intersectsNoLine(posWest) || street->get2DPosition().intersectsNoLine(posSouth))
            {
                isBordering = true;
                break;
            }
        }
        return isBordering;
    }

    void City::generate(unsigned int seed, std::shared_ptr<GameMap> gameMap, long people)
    {
        this->gameMap = gameMap;
        std::mt19937 gen(seed);
        if (people == 0)
            people = 100000;

        std::uniform_int_distribution<long> xPositionGen(0, gameMap->getWidth());
        std::uniform_int_distribution<long> yPositionGen(0, gameMap->getHeight());

        std::uniform_int_distribution<long> peopleGen(people * 0.5, people * 1.5);

        std::uniform_int_distribution<long> houseGen(1, 2);

        numberOfCitizen = peopleGen(gen);
        long numberOfBuildings = static_cast<long>(std::round(static_cast<float>(numberOfCitizen) / 1000.0f));
        APP_LOG_TRACE("city name: " + this->name);
        APP_LOG_TRACE("numberOfCitizen = " + std::to_string(numberOfCitizen));
        APP_LOG_TRACE("numberOfBuildings = " + std::to_string(numberOfBuildings));

        graphics::Rect buildRect = {position.getX(), position.getY(), 1, 1};

        while (!gameMap->canBuild(buildRect))
        {
            position = utils::Vector2(xPositionGen(gen), yPositionGen(gen));
            buildRect = {position.getX(), position.getY(), 1, 1};
        }
        utils::Vector2 startPosition = position;

        world::buildings::DemandMap baseDemand;
        baseDemand[world::ProductType::Food] = 0.7f;
        // baseDemand[world::ProductType::Resource] = 0.3f;

        generateStreetTree(seed);
        fillStreetsByLines();

        for (auto street : streets)
        {
            std::shared_ptr<Building> house = nullptr;
            utils::Vector2 housePosition(street->getDisplayRect().x, street->getDisplayRect().y);

            // change it in the real version
            int houseId = 1;
            std::string subTexture = "house_" + std::to_string(houseId);
            graphics::Rect srcRect;

            bool tryBiggerHouse = true;
            float factor = 4;
            {
                house = std::make_shared<Building>("House", _("House"), "", 100, BuildingType::House, 2, 2);
                house->setPosition(housePosition.getX(), housePosition.getY());

                subTexture = "house_2x2_" + std::to_string(houseId);
                house->setSubTexture(subTexture);

                groundTexture->getSourceRect(subTexture, &srcRect);
                house->setSourceRect(srcRect);

                tryBiggerHouse = findHousePosition(house, street, gen);
            }
            if (!tryBiggerHouse)
            {
                houseId = houseGen(gen);
                factor = 1;
                house = std::make_shared<Building>("House", _("House"), "", 100, BuildingType::House);
                house->setPosition(housePosition.getX(), housePosition.getY());

                subTexture = "house_" + std::to_string(houseId);
                house->setSubTexture(subTexture);
                groundTexture->getSourceRect(subTexture, &srcRect);
                house->setSourceRect(srcRect);
                if (!findHousePosition(house, street, gen))
                    continue;
            }
            auto offset = groundTexture->getOffset(subTexture);
            house->setOffset(offset.getX(), offset.getY());
            float distance = housePosition.distance(this->position);
            distance = (distance == 0) ? 1 : distance;
            int residents = (numberOfCitizen / numberOfBuildings) * (4 / std::sqrt(distance)) * factor;

            std::shared_ptr<world::buildings::HouseComponent> houseComponent = std::make_shared<world::buildings::HouseComponent>(residents, baseDemand, houseId);
            houseComponent->updateTexture(house.get());
            auto component = std::dynamic_pointer_cast<world::buildings::BuildingComponent>(houseComponent);
            house->addComponent(component);

            buildings.push_back(house);
        }

        std::sort(streets.begin(), streets.end(), [&](std::shared_ptr<world::buildings::Street> &o1, std::shared_ptr<world::buildings::Street> &o2)
                  {
                      utils::Vector2 v1(o1->get2DPosition().x, o1->get2DPosition().y);
                      auto v11 = iso::twoDToIso(v1);

                      utils::Vector2 v2(o2->get2DPosition().x, o2->get2DPosition().y);
                      auto v22 = iso::twoDToIso(v2);

                      return v11.getY() < v22.getY(); });

        std::sort(buildings.begin(), buildings.end(), [&](std::shared_ptr<world::Building> o1, std::shared_ptr<world::Building> o2)
                  {
                      utils::Vector2 v1(o1->get2DPosition().x, o1->get2DPosition().y);
                      auto v11 = iso::twoDToIso(v1);

                      utils::Vector2 v2(o2->get2DPosition().x, o2->get2DPosition().y);
                      auto v22 = iso::twoDToIso(v2);

                      return v11.getY() < v22.getY(); });

        for (auto &street : streets)
        {
            gameMap->addBuilding(street);
        }
        for (auto &building : buildings)
        {
            gameMap->addBuilding(building);
        }
    }

    void City::renderCity(core::Renderer *renderer)
    {
        float factor = ceilf(renderer->getZoomFactor() * 100) / 100;

        float x = static_cast<float>(position.getX()) * 64.f / 2.0f;
        float y = static_cast<float>(position.getY()) * 32.f;
        utils::Vector2 vec(x, y);
        auto isoPos = iso::twoDToIso(vec);
        const utils::Vector2 isoPos2((isoPos.getX() * factor) - renderer->getMainCamera()->getX(), (isoPos.getY() * factor) - renderer->getMainCamera()->getY());
        if (renderer->getViewPort().intersects(isoPos2))
            font->render(renderer, name, utils::color::WHITE, isoPos2.getX(), isoPos2.getY());
    }

    void City::fillStreetsByLines()
    {
        graphics::Rect srcRect;

        auto street = std::make_shared<world::buildings::Street>();
        // todo change position, or move to new class
        groundTexture->getSourceRect("street1", &srcRect);

        for (auto child : streetLines)
        {
            int xOffset = 0;
            int yOffset = 0;
            if (child.start.getY() < child.end.getY())
            { // North
                yOffset = 1;
            }
            else if (child.start.getY() > child.end.getY())
            { // south
                yOffset = -1;
            }
            else if (child.start.getX() < child.end.getX())
            { // east
                xOffset = 1;
            }
            else if (child.start.getX() > child.end.getX())
            { // west
                xOffset = -1;
            }

            for (int posY = child.start.getY(); posY != child.end.getY() || !yOffset; posY += yOffset)
            {
                for (int posX = child.start.getX(); posX != child.end.getX() || !xOffset; posX += xOffset)
                {
                    auto street2 = std::make_shared<world::buildings::Street>();
                    // todo change position, or move to new class

                    street2->setSourceRect(srcRect);

                    street2->setOffset(0, 0);

                    street2->setPosition(posX, posY);
                    street2->setSubTexture("street1");
                    if (!isBlocked(street2->get2DPosition(), gameMap))
                        streets.push_back(street2);

                    if (!xOffset)
                        break;
                }

                if (!yOffset)
                    break;
            }
        }

        // for (auto child : node->children)
        // {
        //     fillStreetsByTree(child);
        // }
    }

    void City::addChildStreets(std::mt19937 generator, StreetLine &baseLine, bool addFurther)
    {

        int xOffset = 0;
        int yOffset = 0;
        std::uniform_int_distribution<int> lineGen(4, 20);
        std::uniform_int_distribution<int> chanceGen(0, 100);

        if (baseLine.start.getY() < baseLine.end.getY())
        { // North
            yOffset = 4;
        }
        else if (baseLine.start.getY() > baseLine.end.getY())
        { // south
            yOffset = -4;
        }
        else if (baseLine.start.getX() < baseLine.end.getX())
        { // east
            xOffset = 4;
        }
        else if (baseLine.start.getX() > baseLine.end.getX())
        { // west
            xOffset = -4;
        }
        APP_LOG_INFO("xOffset = " + std::to_string(xOffset));
        APP_LOG_INFO("yOffset = " + std::to_string(yOffset));

        for (int posY = baseLine.start.getY(); posY != baseLine.end.getY() || !yOffset; posY += yOffset)
        {
            for (int posX = baseLine.start.getX(); posX != baseLine.end.getX() || !xOffset; posX += xOffset)
            {

                utils::Vector2 start = {float(posX), float(posY)};
                // first Direction
                if (chanceGen(generator) >= 50)
                {
                    utils::Vector2 end = start;
                    if (yOffset != 0)
                        end += utils::Vector2(lineGen(generator), 0);
                    else
                        end += utils::Vector2(0, lineGen(generator));

                    StreetLine line = {start, end};
                    streetLines.push_back(line);
                    if (addFurther)
                        addChildStreets(generator, line, false);
                }

                // second direction
                if (chanceGen(generator) >= 50)
                {
                    utils::Vector2 end = start;
                    if (yOffset != 0)
                        end += utils::Vector2(lineGen(generator) * -1, 0);
                    else
                        end += utils::Vector2(0, lineGen(generator) * -1);
                    StreetLine line = {start, end};
                    streetLines.push_back(line);
                    if (addFurther)
                        addChildStreets(generator, line, false);
                }
                if (!xOffset || ((posX > baseLine.end.getX() && xOffset > 0) || (posX < baseLine.end.getX() && xOffset < 0)))
                    break;
            }

            if (!yOffset || ((posY > baseLine.end.getY() && yOffset > 0) || (yOffset < 0 && posY < baseLine.end.getY())))
                break;
        }
    }

    bool City::findHousePosition(std::shared_ptr<Building> &house, std::shared_ptr<world::buildings::Street> &street, std::mt19937 &gen)
    {
        int stopKey = 0;
        utils::Vector2 housePosition;
        std::uniform_int_distribution<int> directionGen(1, 4);

        while (isBlocked(house->get2DPosition(), gameMap) && stopKey < 10)
        {
            housePosition = utils::Vector2(street->getDisplayRect().x, street->getDisplayRect().y);

            do
            {

                switch (directionGen(gen))
                {
                case 1: // north
                    housePosition = housePosition - utils::Vector2(0, 1);
                    break;
                case 2: // south
                    housePosition = housePosition + utils::Vector2(0, 1);
                    break;
                case 3: // east
                    housePosition = housePosition - utils::Vector2(1, 0);
                    break;
                case 4: // west
                    housePosition = housePosition + utils::Vector2(1, 0);
                    break;
                }
                house->setPosition(housePosition.getX(), housePosition.getY());
            } while (!isBorderingStreet(house) && stopKey < 10);
            stopKey++;
        }
        return !isBlocked(house->get2DPosition(), gameMap);
    }

    void City::generateStreetTree(unsigned int seed)
    {
        std::mt19937 gen(seed);
        long numberOfBuildings = long(std::round(float(numberOfCitizen) / 200.0f));
        APP_LOG_ERROR(name + " numberOfBuildings = " + std::to_string(numberOfBuildings));
        int maxStreetLength = std::max(numberOfBuildings / 25L, 4L);
        int minStreetLegnth = maxStreetLength * 0.75;
        ;
        std::uniform_int_distribution<int> lineGen(minStreetLegnth, maxStreetLength);

        for (int direction = 1; direction <= 4; direction++)
        {
            utils::Vector2 streetPosition = position;
            int distance = lineGen(gen);
            switch (direction)
            {
            case 1: // north
                streetPosition += utils::Vector2(0, distance);
                break;
            case 2: // south
                streetPosition -= utils::Vector2(0, distance);
                break;
            case 3: // east
                streetPosition = streetPosition + utils::Vector2(distance, 0);
                break;
            case 4: // west
                streetPosition = streetPosition - utils::Vector2(distance, 0);
                break;
            }
            StreetLine line = {position, streetPosition};
            streetLines.push_back(line);
            APP_LOG_INFO("distance = " + std::to_string(distance));
            addChildStreets(gen, line, distance >= 4);
        }
    }

    std::shared_ptr<utils::JSON::Object> City::toJson()
    {
        std::shared_ptr<utils::JSON::Object> json = std::make_shared<utils::JSON::Object>();

        json->setAttribute("name", name);
        json->setAttribute("numberOfCitizen", int(numberOfCitizen));
        // json->setAttribute("type",   type);
        json->setAttribute("pos_x", position.getX());
        json->setAttribute("pos_y", position.getY());

        utils::JSON::JsonArray jsonStreets;

        for (auto &street : streets)
        {
            utils::JSON::JsonValue value = street->toJson();
            jsonStreets.push_back(value);
        }
        json->setArrayAttribute("streets", jsonStreets);
        utils::JSON::JsonArray jsonBuildings;
        for (auto &street : buildings)
        {
            utils::JSON::JsonValue value = street->toJson();
            jsonBuildings.push_back(value);
        }
        json->setArrayAttribute("buildings", jsonBuildings);
        return json;
    }

    std::shared_ptr<City> City::fromJson(const std::shared_ptr<utils::JSON::Object> &object)
    {
        std::string name = object->getStringValue("name");
        int numberOfCitizen = object->getIntValue("numberOfCitizen");
        int posX = object->getFloatValue("pos_x");
        int posY = object->getFloatValue("pos_y");
        utils::Vector2 pos(posX, posY);
        auto city = std::make_shared<City>(name, pos);
        city->addCitizen(numberOfCitizen);
        utils::JSON::JsonArray jsonStreets = object->getArray("streets");
        for (auto s : jsonStreets)
        {
            auto streetObject = std::get<std::shared_ptr<utils::JSON::Object>>(s);
            auto street = std::make_shared<world::buildings::Street>();
            // todo change position, or move to new class
            graphics::Rect srcRect;
            city->groundTexture->getSourceRect("street1", &srcRect);
            street->setSourceRect(srcRect);

            street->setOffset(0, 0);
            street->setPosition(streetObject->getFloatValue("pos_x"), streetObject->getFloatValue("pos_y"));
            street->setSubTexture("street1");
            city->streets.push_back(street);
        }

        utils::JSON::JsonArray jsonBuildings = object->getArray("buildings");

        for (auto b : jsonBuildings)
        {
            auto buildingObject = std::get<std::shared_ptr<utils::JSON::Object>>(b);

            auto house = std::make_shared<Building>("House", _("House"), "", 100, BuildingType::House);
            house = Building::fromJson(house, buildingObject, nullptr);
            // utils::Vector2 housePosition(buildingObject->getFloatValue("pos_x"), buildingObject->getFloatValue("pos_y"));
            // //change it in the real version
            std::string subTexture = buildingObject->getStringValue("subTexture");
            graphics::Rect srcRect;
            city->groundTexture->getSourceRect(subTexture, &srcRect);
            house->setSourceRect(srcRect);
            // house->setPosition(housePosition.getX(), housePosition.getY());
            int xOffset = buildingObject->getIntValue("offset_x");
            int yOffset = buildingObject->getIntValue("offset_y");

            house->setOffset(xOffset, yOffset);
            // house->setSubTexture(subTexture);
            city->buildings.push_back(house);
        }
        return city;
    }

    std::vector<std::shared_ptr<Building>> &City::getBuildings()
    {
        return buildings;
    }

    std::vector<std::shared_ptr<world::buildings::Street>> &City::getStreets()
    {
        return streets;
    }

    long City::getNumberOfCitizen()
    {
        return numberOfCitizen;
    }

    utils::Vector2 &City::getPosition()
    {
        return position;
    }
    std::string &City::getName()
    {
        return name;
    }
}
