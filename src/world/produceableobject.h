#pragma once

#include <string>

class ProduceableObject
{
private:
    /* data */
public:
    ProduceableObject(const std::string &name, const std::string &image);
    ~ProduceableObject();

    std::string getName();
    std::string getImage();

private:
    std::string name;
    std::string image;
};