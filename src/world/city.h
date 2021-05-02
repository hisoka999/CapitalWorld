#ifndef CITY_H
#define CITY_H

#include <vector>
#include "../world/building.h"
#include "../world/gamemap.h"
#include <memory>
#include <engine/core/renderer.h>
#include <engine/graphics/texturemap.h>
#include <random>

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
    };

    class City
    {
    public:
        City(std::string name, utils::Vector2 &position);
        void generate(unsigned int seed, std::shared_ptr<GameMap> gameMap);
        void renderCity(core::Renderer *renderer);

    private:
        bool isBlocked(graphics::Rect rect, const std::shared_ptr<GameMap> &gameMap);
        bool isOverlapStreet(std::shared_ptr<Building> &building);
        bool isBorderingStreet(std::shared_ptr<Building> &building);
        void generateStreetTree(unsigned int seed);
        void fillStreetsByTree(std::shared_ptr<TreeNode> node);
        void fillNode(std::mt19937 &gen, std::shared_ptr<TreeNode> node, long *nodesLeft);
        bool existsNode(std::shared_ptr<TreeNode> node, utils::Vector2 &pos);
        void updateObjectList();
        void updateStreetTextures();

        utils::Vector2 position;
        std::string name;
        long numberOfCitizen;
        CityType type;
        std::shared_ptr<GameMap> gameMap;
        std::shared_ptr<graphics::TextureMap> groundTexture;
        std::vector<std::shared_ptr<Building>> buildings;
        std::vector<std::shared_ptr<Building>> streets;
        std::vector<std::shared_ptr<Building>> objects;
        std::shared_ptr<TreeNode> root;
    };
}

#endif // CITY_H
