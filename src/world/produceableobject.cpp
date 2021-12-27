#include "produceableobject.h"

ProduceableObject::ProduceableObject(const std::string &name, const std::string &image) : name(name), image(image)
{
}

ProduceableObject::~ProduceableObject()
{
}

std::string ProduceableObject::getName()
{
    return name;
}

std::string ProduceableObject::getImage()
{
    return image;
}
