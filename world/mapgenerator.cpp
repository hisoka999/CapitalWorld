#include "mapgenerator.h"
#include <engine/utils/perlinnoise.h>
#include <iostream>
#include <random>

namespace world {

MapGenerator::MapGenerator()
{

}

std::shared_ptr<GameMap> MapGenerator::generateMap(size_t width,size_t height)
{
    generateMap(width,height,1);
}
std::shared_ptr<GameMap> MapGenerator::generateMap(size_t width,size_t height,unsigned int seed)
{
    std::cout<<"seed: "<<seed<<std::endl;
    utils::PerlinNoise pn(seed);
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int>   dist(-10,20);
    std::vector<TileType> mapData;
    mapData.resize(width*height);
    std::fill(mapData.begin(),mapData.end(),10);


    // Visit every pixel of the image and assign a color generated with Perlin noise
    for(unsigned int i = 0; i < height; ++i) {     // y
        for(unsigned int j = 0; j < width; ++j) {  // x
            double x = (double)j/((double)width);
            double y = (double)i/((double)height);
            int z = dist(gen);
            double base = 10;
            double noise = 20*pn.noise(base*x,base*y,0.5);
            //noise = noise-std::floor(noise );
            //noise = noise*20;
            std::cout<<"x: "<<x<<" y: "<<y<<" z: "<<z<<" noise: "<<noise<<std::endl;

            mapData[i+(j*height)]= std::floor(noise);
        }
    }
    return std::make_shared<GameMap>(width,height,mapData);
}

}
