#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "world/gamemap.h"
#include <memory>

namespace world {


class MapGenerator
{
public:
    MapGenerator();

    std::shared_ptr<GameMap> generateMap(size_t width,size_t height);
    std::shared_ptr<GameMap> generateMap(size_t width,size_t height,unsigned int seed);
};

}

#endif // MAPGENERATOR_H
