#include "city.h"

#include <algorithm>
#include <cmath>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#include <iostream>
#include <random>

namespace world
{

    City::City(std::string name, utils::Vector2 &position)
        : position(position), name(name), numberOfCitizen(0)
    {
        groundTexture = graphics::TextureManager::Instance().loadTextureMap(utils::os::combine("images", "tiles", "iso_tiles.tm"));
    }

    bool City::isBlocked(graphics::Rect rect, std::shared_ptr<GameMap> gameMap)
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
    }

    void City::generate(unsigned int seed, std::shared_ptr<GameMap> gameMap)
    {
        this->gameMap = gameMap;
        std::mt19937 gen(seed);

        std::uniform_int_distribution<long> xPositionGen(0, gameMap->getWidth());
        std::uniform_int_distribution<long> yPositionGen(0, gameMap->getHeight());

        std::uniform_int_distribution<long> peopleGen(100000, 1000000);

        std::uniform_int_distribution<long> houseGen(1, 4);

        numberOfCitizen = peopleGen(gen);
        long numberOfBuildings = static_cast<long>(std::round(static_cast<float>(numberOfCitizen) / 1000.0f));
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

        generateStreetTree(seed);
        fillStreetsByTree(root);

        for (auto street : streets)
        {
            auto house = std::make_shared<Building>("House", "", 100, BuildingType::House);
            utils::Vector2 housePosition(street->getDisplayRect().x, street->getDisplayRect().y);
            //change it in the real version
            int houseId = houseGen(gen);
            std::string subTexture = "house" + std::to_string(houseId);
            house->setSourceRect(groundTexture->getSourceRect(subTexture));
            house->setPosition(housePosition.getX(), housePosition.getY());
            house->setOffset(0, house->getSourceRect().height - height);
            house->setSubTexture(subTexture);
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
                    std::cout << "house x = " << housePosition.getX() << " y = " << housePosition.getY() << std::endl;
                } while (!isBorderingStreet(house) && stopKey < 10);
                stopKey++;
            }
            if (stopKey < 10)
                buildings.push_back(house);
        }

        updateObjectList();
    }

    void City::fillStreetsByTree(std::shared_ptr<TreeNode> node)
    {
        int height = 28;
        auto street = std::make_shared<Building>("Street", "", 100, BuildingType::Street);
        //todo change position, or move to new class
        street->setSourceRect(graphics::Rect(1044, 0, 64, 32));
        street->setOffset(0, 0);
        utils::Vector2 streetPosition = node->position;
        street->setPosition(streetPosition.getX(), streetPosition.getY());
        street->setSubTexture("street1");
        streets.push_back(street);

        auto street2 = std::make_shared<Building>("Street", "", 100, BuildingType::Street);
        //todo change position, or move to new class
        street2->setSourceRect(graphics::Rect(1044, 0, 64, 32));
        street2->setOffset(0, 0);
        streetPosition = node->position;
        switch (node->direction)
        {
        case 1: //north
            streetPosition = streetPosition + utils::Vector2(0, 1);
            break;
        case 2: //south
            streetPosition = streetPosition - utils::Vector2(0, 1);
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
        streets.push_back(street2);

        for (auto child : node->children)
        {
            fillStreetsByTree(child);
        }
    }
    void City::updateStreetTextures()
    {
        for (auto &sourceStreet : streets)
        {
            bool isBorderingNorth = false;
            bool isBorderingSouth = false;
            bool isBorderingEast = false;
            bool isBorderingWest = false;
            auto pos = sourceStreet->get2DPosition();
            auto posNorth = pos;
            posNorth.y -= 1;
            auto posEast = pos;
            posEast.x += 1;
            auto posWest = pos;
            posWest.x -= 1;
            auto posSouth = pos;
            posSouth.y += 1;

            for (auto &street : streets)
            {
                if (street->get2DPosition().intersectsNoLine(posNorth))
                    isBorderingNorth = true;
                if (street->get2DPosition().intersectsNoLine(posSouth))
                    isBorderingSouth = true;
                if (street->get2DPosition().intersectsNoLine(posEast))
                    isBorderingEast = true;
                if (street->get2DPosition().intersectsNoLine(posWest))
                    isBorderingWest = true;
            }
            if (isBorderingEast && isBorderingSouth && isBorderingNorth && isBorderingWest)
            {
                sourceStreet->setSubTexture("street_cross_center");
            }
            else if (isBorderingEast && isBorderingSouth && !isBorderingNorth && isBorderingWest)
            {
                sourceStreet->setSubTexture("street_cross_up_right");
            }
            else if (isBorderingEast && isBorderingSouth && isBorderingNorth && !isBorderingWest)
            {
                sourceStreet->setSubTexture("street_cross_up_left");
            }
            else if (isBorderingEast && !isBorderingSouth && isBorderingNorth && isBorderingWest)
            {
                sourceStreet->setSubTexture("street_cross_down_left");
            }
            else if (!isBorderingEast && isBorderingSouth && isBorderingNorth && isBorderingWest)
            {
                sourceStreet->setSubTexture("street_cross_down_right");
            }
            else if (isBorderingSouth && isBorderingEast && !isBorderingNorth && !isBorderingWest)
            {
                sourceStreet->setSubTexture("street_corner_up");
            }
            else if (isBorderingNorth && !isBorderingEast && isBorderingWest && !isBorderingSouth)
            {
                sourceStreet->setSubTexture("street_corner_left");
            }
            else if (!isBorderingNorth && !isBorderingEast && isBorderingWest && isBorderingSouth)
            {
                sourceStreet->setSubTexture("street_corner_right");
            }
            else if (isBorderingNorth && isBorderingEast && !isBorderingWest && !isBorderingSouth)
            {
                sourceStreet->setSubTexture("street_corner_down");
            }
            else if (isBorderingEast || isBorderingWest)
            {
                sourceStreet->setSubTexture("street2");
            }
            else
            {
                sourceStreet->setSubTexture("street1");
            }
        }
    }
    void City::generateStreetTree(unsigned int seed)
    {
        root = std::make_shared<TreeNode>(position, 0);
        std::mt19937 gen(seed);
        long numberOfBuildings = long(std::round(float(numberOfCitizen) / 1000.0f));
        numberOfBuildings /= 2.0;
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
        if (root->children.size() > 0)
        {
            base = 1;
        }

        if ((*nodesLeft) == 0)
            return;
        std::cout << "nodes left: " << *nodesLeft << std::endl;

        std::uniform_int_distribution<int> directionGen(base, 4);
        std::uniform_int_distribution<int> noDirectionGen(1, 100);

        int directions = directionGen(gen);
        std::cout << "directions:" << directions << std::endl;

        if (root->children.size() > 0)
        {
            if (noDirectionGen(gen) <= 10)
            {
                std::cout << " no direction " << std::endl;
                return;
            }
        }
        std::vector<std::shared_ptr<TreeNode>> children;

        for (int i = 1; i <= directions; ++i)
        {
            utils::Vector2 streetPosition = node->position;

            switch (i)
            {
            case 1: //north
                streetPosition = streetPosition - utils::Vector2(0, 2);
                break;
            case 2: //south
                streetPosition = streetPosition + utils::Vector2(0, 2);
                break;
            case 3: //east
                streetPosition = streetPosition - utils::Vector2(2, 0);
                break;
            case 4: //west
                streetPosition = streetPosition + utils::Vector2(2, 0);
                break;
            }
            //don't fill existing childs

            if (i != node->direction)
            {
                //std::cout << "street pos:" << streetPosition.getX() << " y : " << streetPosition.getY() << std::endl;

                //first check if you can build a street
                graphics::Rect buildRect{streetPosition.getX(), streetPosition.getY(), 1, 1};
                if (gameMap->canBuild(buildRect))
                {
                    auto child = std::make_shared<TreeNode>(streetPosition, i);
                    (*nodesLeft)--;

                    children.push_back(child);
                    if ((*nodesLeft) == 0)
                        break;
                }
            }
            else
            {
                if (directions < 4)
                    directions++;
            }
        }
        for (auto child : children)
        {
            if (!existsNode(root, child->position))
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

    void City::updateObjectList()
    {
        objects.clear();
        objects.insert(objects.end(), streets.begin(), streets.end());

        objects.insert(objects.end(), buildings.begin(), buildings.end());

        std::sort(objects.begin(), objects.end(), [&](std::shared_ptr<world::Building> o1, std::shared_ptr<world::Building> o2) {
            utils::Vector2 v1(o1->get2DPosition().x, o1->get2DPosition().y);
            auto v11 = gameMap->twoDToIso(v1);

            utils::Vector2 v2(o2->get2DPosition().x, o2->get2DPosition().y);
            auto v22 = gameMap->twoDToIso(v2);

            return v11.getY() < v22.getY();
            //  o1->get2DPosition().x > o2->get2DPosition().x
            ;
        });
        updateStreetTextures();
    }

    void City::renderCity(core::Renderer *renderer)
    {
        int tileWidth = 64;
        int tileHeight = 32;
        auto camera = renderer->getMainCamera();

        for (auto &building : objects)
        {
            auto displayRect = building->getDisplayRect();

            displayRect.x = (displayRect.x * tileWidth / 2.0f);
            displayRect.y = (displayRect.y * tileHeight);

            utils::Vector2 vec(displayRect.x, displayRect.y);
            auto pos = gameMap->twoDToIso(vec);
            displayRect.x = pos.getX() * renderer->getZoomFactor();
            displayRect.y = pos.getY() * renderer->getZoomFactor();
            displayRect.width *= renderer->getZoomFactor();
            displayRect.height *= renderer->getZoomFactor();
            if (!displayRect.intersects(camera->getViewPortRect()))
                continue;

            displayRect.x = ((pos.getX() - building->getXOffset()) * renderer->getZoomFactor()) - camera->getX();
            displayRect.y = ((pos.getY() - building->getYOffset()) * renderer->getZoomFactor()) - camera->getY();
            //groundTexture->render(renderer,building->getSourceRect(),displayRect);

            groundTexture->render(building->getSubTexture(), displayRect, renderer);
        }
    }

}
