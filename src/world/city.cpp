#include "city.h"

#include <algorithm>
#include <cmath>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#include <iostream>
#include <random>
#include "../translate.h"
#include "world/buildings/HouseComponent.h"
#include <engine/utils/color.h>
#include "iso.h"

namespace world
{

    City::City(std::string name, utils::Vector2 &position)
        : position(position), name(name), numberOfCitizen(0)
    {
        groundTexture = graphics::TextureManager::Instance().loadTextureMap(utils::os::combine("images", "tiles", "iso_tiles.json"));
        font = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 20);
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
        if (!gameMap->canBuild(rect))
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
        posNorth.y -= 1;
        auto posEast = pos;
        posEast.x -= 1;
        auto posWest = pos;
        posWest.x += 1;
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
        std::cout << "city name: " << this->name << std ::endl;
        std::cout << "numberOfCitizen = " << numberOfCitizen << std::endl;
        std::cout << "numberOfBuildings = " << numberOfBuildings << std::endl;

        graphics::Rect buildRect = {position.getX(), position.getY(), 1, 1};

        while (!gameMap->canBuild(buildRect))
        {
            position = utils::Vector2(xPositionGen(gen), yPositionGen(gen));
            buildRect = {position.getX(), position.getY(), 1, 1};
        }
        utils::Vector2 startPosition = position;

        std::uniform_int_distribution<int> directionGen(1, 4);

        int width = 64;
        int height = 32;
        world::buildings::DemandMap baseDemand;
        baseDemand[world::ProductType::Food] = 0.7f;

        generateStreetTree(seed);
        fillStreetsByTree(root);

        for (auto street : streets)
        {
            auto house = std::make_shared<Building>("House", _("House"), "", 100, BuildingType::House);
            utils::Vector2 housePosition(street->getDisplayRect().x, street->getDisplayRect().y);
            //change it in the real version
            int houseId = houseGen(gen);
            std::string subTexture = "house_" + std::to_string(houseId);

            house->setSourceRect(groundTexture->getSourceRect(subTexture));
            house->setPosition(housePosition.getX(), housePosition.getY());
            house->setOffset(0, house->getSourceRect().height - height);

            house->setSubTexture(subTexture);

            float distance = housePosition.distance(this->position);

            int residents = (numberOfCitizen / numberOfBuildings) * ((distance == 0) ? 1 : (5.f / distance));

            std::shared_ptr<world::buildings::BuildingComponent> houseComponent = std::make_shared<world::buildings::HouseComponent>(residents, baseDemand, houseId);
            houseComponent->updateProduction(0, 0, house.get());
            house->addComponent(houseComponent);
            int stopKey = 0;
            while (isBlocked(house->get2DPosition(), gameMap) && stopKey < 10)
            {
                housePosition = utils::Vector2(street->getDisplayRect().x, street->getDisplayRect().y);

                do
                {

                    switch (directionGen(gen))
                    {
                    case 1: //north
                        housePosition = housePosition - utils::Vector2(0, 1);
                        break;
                    case 2: //south
                        housePosition = housePosition + utils::Vector2(0, 1);
                        break;
                    case 3: //east
                        housePosition = housePosition - utils::Vector2(1, 0);
                        break;
                    case 4: //west
                        housePosition = housePosition + utils::Vector2(1, 0);
                        break;
                    }
                    house->setPosition(housePosition.getX(), housePosition.getY());
                } while (!isBorderingStreet(house) && stopKey < 10);
                stopKey++;
            }
            if (stopKey < 10)
                buildings.push_back(house);
        }

        std::sort(streets.begin(), streets.end(), [&](std::shared_ptr<world::buildings::Street> &o1, std::shared_ptr<world::buildings::Street> &o2)
                  {
                      utils::Vector2 v1(o1->get2DPosition().x, o1->get2DPosition().y);
                      auto v11 = iso::twoDToIso(v1);

                      utils::Vector2 v2(o2->get2DPosition().x, o2->get2DPosition().y);
                      auto v22 = iso::twoDToIso(v2);

                      return v11.getY() < v22.getY();
                      //  o1->get2DPosition().x > o2->get2DPosition().x
                      ;
                  });

        std::sort(buildings.begin(), buildings.end(), [&](std::shared_ptr<world::Building> o1, std::shared_ptr<world::Building> o2)
                  {
                      utils::Vector2 v1(o1->get2DPosition().x, o1->get2DPosition().y);
                      auto v11 = iso::twoDToIso(v1);

                      utils::Vector2 v2(o2->get2DPosition().x, o2->get2DPosition().y);
                      auto v22 = iso::twoDToIso(v2);

                      return v11.getY() < v22.getY();
                      //  o1->get2DPosition().x > o2->get2DPosition().x
                      ;
                  });

        for (auto &street : streets)
        {
            gameMap->addBuilding(street);
        }
        for (auto &street : buildings)
        {
            gameMap->addBuilding(street);
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

    void City::fillStreetsByTree(std::shared_ptr<TreeNode> node)
    {
        int height = 28;
        auto street = std::make_shared<world::buildings::Street>();
        //todo change position, or move to new class
        street->setSourceRect(groundTexture->getSourceRect("street1"));
        street->setOffset(0, 0);
        utils::Vector2 streetPosition = node->position;

        street->setPosition(streetPosition.getX(), streetPosition.getY());
        street->setSubTexture("street1");

        if (!isBlocked(street->get2DPosition(), gameMap))
            streets.push_back(street);

        auto street2 = std::make_shared<world::buildings::Street>();
        //todo change position, or move to new class

        street2->setSourceRect(groundTexture->getSourceRect("street1"));
        street2->setOffset(0, 0);
        streetPosition = node->position;
        switch (node->direction)
        {
        case 1: //north
            streetPosition += utils::Vector2(0, 1);
            break;
        case 2: //south
            streetPosition -= utils::Vector2(0, 1);
            break;
        case 3: //east
            streetPosition = streetPosition + utils::Vector2(1, 0);
            break;
        case 4: //west
            streetPosition = streetPosition - utils::Vector2(1, 0);
            break;
        }

        street2->setPosition(streetPosition.getX(), streetPosition.getY());
        street2->setSubTexture("street1");
        if (!isBlocked(street2->get2DPosition(), gameMap))
            streets.push_back(street2);

        for (auto child : node->children)
        {
            fillStreetsByTree(child);
        }
    }

    void City::generateStreetTree(unsigned int seed)
    {
        root = std::make_shared<TreeNode>(position, 0);
        std::mt19937 gen(seed);
        long numberOfBuildings = long(std::round(float(numberOfCitizen) / 1000.0f));
        //numberOfBuildings /= 2.0;
        fillNode(gen, root, &numberOfBuildings);
    }

    bool City::existsNode(std::shared_ptr<TreeNode> node, utils::Vector2 &pos)
    {

        if (node->position == pos)
            return true;
        for (auto child : node->children)
        {
            if (existsNode(child, pos))
            {
                return true;
            }
        }
        return false;
    }

    void City::fillNode(std::mt19937 &gen, std::shared_ptr<TreeNode> node, long *nodesLeft)
    {

        int base = 4;
        int max = 4;
        if (root->children.size() > 0)
        {
            base = 2;
            max = 3;
        }

        if ((*nodesLeft) <= 0)
            return;
        //std::cout << "nodes left: " << *nodesLeft << std::endl;

        std::uniform_int_distribution<int> directionGen(base, max);
        std::uniform_int_distribution<int> noDirectionGen(1, 100);

        int directions = directionGen(gen);
        //std::cout << "directions:" << directions << std::endl;

        if (root->children.size() > 0)
        {
            if (noDirectionGen(gen) <= 1)
            {
                std::cout << " no direction " << std::endl;
                return;
            }
        }
        std::vector<std::shared_ptr<TreeNode>> children;

        std::vector<int> usedDirections;
        for (int i = 1; i <= 4; ++i)
        {
            if (i != node->direction)
            {
                usedDirections.push_back(i);
            }
        }
        std::shuffle(usedDirections.begin(), usedDirections.end(), gen);

        for (int i = 1; i <= directions; ++i)
        {
            if ((*nodesLeft) == 0)
                break;
            size_t directionNodes = 2;
            int direction = usedDirections[i - 1];

            for (size_t dn = 0; dn < directionNodes; ++dn)
            {
                utils::Vector2 streetPosition = node->position;

                if ((*nodesLeft) == 0)
                    break;
                switch (direction)
                {
                case 1: //north
                    streetPosition -= utils::Vector2(0.f, 2.f + float(dn));
                    break;
                case 2: //south
                    streetPosition += utils::Vector2(0.f, 2.f + float(dn));
                    break;
                case 3: //east
                    streetPosition -= utils::Vector2(2.f + float(dn), 0.f);
                    break;
                case 4: //west
                    streetPosition += utils::Vector2(2.f + float(dn), 0.f);
                    break;
                }
                //don't fill existing childs

                //std::cout << "street pos:" << streetPosition.getX() << " y : " << streetPosition.getY() << std::endl;

                graphics::Rect buildRect{streetPosition.getX(), streetPosition.getY(), 1, 1};
                if (gameMap->canBuild(buildRect))
                {
                    auto child = std::make_shared<TreeNode>(streetPosition, direction);
                    child->lastNode = dn + 1 == directionNodes;
                    (*nodesLeft)--;

                    children.push_back(child);
                    if ((*nodesLeft) == 0)
                        break;
                }

                //first check if you can build a street
                // }
                // else
                // {
                //     if (directions < 4)
                //         directions++;
                // }
            }
        }
        for (auto child : children)
        {
            if (!existsNode(root, child->position) && child->lastNode)
            {
                node->children.push_back(child);
                fillNode(gen, child, nodesLeft);
            }
            else
            {
                node->children.push_back(child);
            }
        }
    }

    std::shared_ptr<utils::JSON::Object> City::toJson()
    {
        std::shared_ptr<utils::JSON::Object> json = std::make_shared<utils::JSON::Object>();

        json->setAttribute("name", name);
        json->setAttribute("numberOfCitizen", int(numberOfCitizen));
        //json->setAttribute("type",   type);
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

        utils::JSON::JsonArray jsonStreets = object->getArray("streets");
        for (auto s : jsonStreets)
        {
            auto streetObject = std::get<std::shared_ptr<utils::JSON::Object>>(s);
            auto street = std::make_shared<world::buildings::Street>();
            //todo change position, or move to new class
            street->setSourceRect(city->groundTexture->getSourceRect("street1"));
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
            house->setSourceRect(city->groundTexture->getSourceRect(subTexture));
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
}
