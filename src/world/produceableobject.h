#pragma once

#include <string>

class ProduceableObject
{
private:
    /* data */
public:
    ProduceableObject(const std::string &localizedName, const std::string &name, const std::string &image);
    ~ProduceableObject();

    std::string getLocalisedName() const;
    std::string getName() const;
    std::string getImage() const;

private:
    std::string m_localizedName;
    std::string m_name;
    std::string m_image;
};