#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "world/gamemap.h"
#include "world/city.h"
#include <memory>

namespace world {


class MapGenerator
{
public:
    MapGenerator();

    std::shared_ptr<GameMap> generateMap(size_t width,size_t height);
    std::shared_ptr<GameMap> generateMap(size_t width,size_t height,unsigned int seed);
    std::vector<std::shared_ptr<world::City>> getGeneratedCities();
private:
    std::vector<std::shared_ptr<world::City>> cities;
};

}

#endif // MAPGENERATOR_H
