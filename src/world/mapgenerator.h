#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "../world/gamemap.h"
#include "../world/city.h"
#include <memory>
#include "constants.h"

namespace world
{
    struct CityDefinition
    {
        std::string name;
        int size;
    };

    class MapGenerator
    {
    public:
        MapGenerator();

        std::shared_ptr<GameMap> generateMap(size_t width, size_t height, int numberOfCities, CityNames cityName);
        std::shared_ptr<GameMap> generateMap(size_t width, size_t height, int numberOfCities, CityNames cityName, unsigned long seed);
        std::vector<std::shared_ptr<world::City>> getGeneratedCities();

    private:
        std::vector<CityDefinition> getCityDefinitions(CityNames cityName);
        std::vector<std::shared_ptr<world::City>> cities;
    };

}

#endif // MAPGENERATOR_H
