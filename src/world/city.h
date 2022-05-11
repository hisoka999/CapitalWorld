#ifndef CITY_H
#define CITY_H
#pragma once
#include <engine/core/renderer.h>
#include <engine/graphics/text.h>
#include <engine/graphics/texturemap.h>
#include <memory>
#include <random>
#include <vector>
#include <world/building.h>
#include <world/buildings/street.h>
#include <world/gamemap.h>

namespace world
{

    enum class CityType
    {
        Village,
        SmallTown,
        City,
        Metropolis
    };

    class TreeNode
    {
    public:
        TreeNode(utils::Vector2 &pos, int direction) : position(pos), direction(direction)
        {
        }

        utils::Vector2 position;
        int direction;

        std::vector<std::shared_ptr<TreeNode>> children;
        bool lastNode = false;
    };

    class City
    {
    public:
        City(std::string name, utils::Vector2 &position);
        void generate(unsigned int seed, std::shared_ptr<GameMap> gameMap, long people);
        void renderCity(core::Renderer *renderer);

        std::shared_ptr<utils::JSON::Object> toJson();
        static std::shared_ptr<City> fromJson(const std::shared_ptr<utils::JSON::Object> &object);
        std::vector<std::shared_ptr<Building>> &getBuildings();
        std::vector<std::shared_ptr<world::buildings::Street>> &getStreets();
        void addCitizen(int people);
        long getNumberOfCitizen();
        utils::Vector2 &getPosition();
        std::string &getName();

    protected:
        std::vector<std::shared_ptr<Building>> buildings;
        std::vector<std::shared_ptr<world::buildings::Street>> streets;

    private:
        bool isBlocked(graphics::Rect rect, const std::shared_ptr<GameMap> &gameMap);
        bool isOverlapStreet(std::shared_ptr<Building> &building);
        bool isBorderingStreet(std::shared_ptr<Building> &building);
        void generateStreetTree(unsigned int seed);
        void fillStreetsByTree(std::shared_ptr<TreeNode> node);
        void fillNode(std::mt19937 &gen, std::shared_ptr<TreeNode> node, long *nodesLeft);
        bool existsNode(std::shared_ptr<TreeNode> node, utils::Vector2 &pos);

        utils::Vector2 position;
        std::string name;
        long numberOfCitizen;
        CityType type;
        std::shared_ptr<GameMap> gameMap;
        std::shared_ptr<graphics::TextureMap> groundTexture;

        std::shared_ptr<TreeNode> root;
        std::shared_ptr<graphics::Text> font;
    };
}

#endif // CITY_H
