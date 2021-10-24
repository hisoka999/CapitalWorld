#include "mapgenerator.h"
#include "magic_enum.hpp"
#include <algorithm>
#include <engine/utils/perlinnoise.h>
#include <engine/utils/string.h>
#include <fstream>
#include <iostream>
#include <random>

namespace world
{

    MapGenerator::MapGenerator()
    {
    }

    std::vector<std::shared_ptr<world::City>> MapGenerator::getGeneratedCities()
    {
        return cities;
    }

    std::vector<CityDefinition> MapGenerator::getCityDefinitions(CityNames cityName)
    {
        std::vector<CityDefinition> cityDefinitions;
        std::fstream file;

        std::string filename = "data/cities/";
        filename += magic_enum::enum_name(cityName);
        filename += ".csv";
        std::string line;
        file.open(filename.c_str(), std::ios::in);
        if (!file.is_open())
        {
            throw IOException(filename, "file does not exists");
        }
        int i = 0;
        while (getline(file, line))
        {
            i++;
            if (i == 1)
            {
                continue;
            }

            auto columns = utils::split(line, ",");
            CityDefinition definition;
            definition.name = columns[0];
            definition.size = std::atoi(columns[8].c_str());

            cityDefinitions.push_back(definition);
        }
        //while(!s.empty());
        file.close();
        return cityDefinitions;
    }

    std::shared_ptr<GameMap> MapGenerator::generateMap(size_t width, size_t height, int numberOfCities, CityNames cityName)
    {
        return generateMap(width, height, numberOfCities, cityName, 1);
    }
    std::shared_ptr<GameMap> MapGenerator::generateMap(size_t width, size_t height, int numberOfCities, CityNames cityName, unsigned long seed)
    {
        std::cout << "seed: " << seed << std::endl;
        auto definitions = getCityDefinitions(cityName);

        utils::PerlinNoise pn(seed);
        std::mt19937 gen(seed);
        std::shuffle(definitions.begin(), definitions.end(), gen);
        std::uniform_int_distribution<int> dist(-10, 20);
        std::uniform_int_distribution<int> decoration(static_cast<int>(Decoration::none), static_cast<int>(Decoration::mountain));
        std::vector<TileType> mapData;
        std::vector<TileType> mapDecoration;
        mapData.resize(width * height);
        mapDecoration.resize(width * height);
        std::fill(mapData.begin(), mapData.end(), 10);
        std::fill(mapDecoration.begin(), mapDecoration.end(), 0);

        // Visit every pixel of the image and assign a color generated with Perlin noise
        for (unsigned int i = 0; i < height; ++i)
        { // y
            for (unsigned int j = 0; j < width; ++j)
            { // x
                double x = (double)j / ((double)width);
                double y = (double)i / ((double)height);
                int z = dist(gen);
                double base = 5;
                double noise = 20 * pn.noise(base * x, base * y, 0.5);
                //noise = noise-std::floor(noise );
                //noise = noise*20;
                //std::cout<<"x: "<<x<<" y: "<<y<<" z: "<<z<<" noise: "<<noise<<std::endl;

                mapData[i + (j * height)] = std::floor(noise);

                if (mapData[i + (j * height)] > 8 && mapData[i + (j * height)] <= 12)
                {
                    mapDecoration[i + (j * height)] = decoration(gen);
                }
            }
        }
        // 8 ... 11 = grass

        auto map = std::make_shared<GameMap>(width, height, mapData, mapDecoration);

        std::uniform_int_distribution<long> xPositionGen(0, width);
        std::uniform_int_distribution<long> yPositionGen(0, height);
        for (int i = 0; i < numberOfCities; i++)
        {
            utils::Vector2 cityPos(xPositionGen(gen), yPositionGen(gen));
            auto city = std::make_shared<world::City>(definitions[i].name, cityPos);
            city->generate(seed, map, definitions[i].size);
            cities.push_back(city);
        }

        return map;
    }

}