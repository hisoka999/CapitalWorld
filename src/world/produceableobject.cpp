#include "produceableobject.h"

ProduceableObject::ProduceableObject(const std::string &localizedName, const std::string &name, const std::string &image) : m_localizedName(localizedName), m_name(name), m_image(image)
{
}

ProduceableObject::~ProduceableObject()
{
}

std::string ProduceableObject::getLocalisedName() const
{
    return m_localizedName;
}

std::string ProduceableObject::getName() const
{
    return m_name;
}

std::string ProduceableObject::getImage() const
{
    return m_image;
}
