#include "object.h"
namespace utils {
namespace JSON {

    Object::Object()
    {
    }

    std::string Object::getStringValue(const std::string& attr) const
    {
        return std::get<std::string>(attributes.at(attr));
    }
    int Object::getIntValue(const std::string& attr) const
    {
        return std::get<int>(attributes.at(attr));
    }
    std::shared_ptr<Object> Object::getObjectValue(const std::string& attr) const
    {
        return std::get<std::shared_ptr<Object>>(attributes.at(attr));
    }

    JsonArray Object::getArray(const std::string& attr) const
    {
        return arrayAttributes.at(attr);
    }
    void Object::setArrayAttribute(const std::string& attr, JsonArray array)
    {
        arrayAttributes[attr] = array;
    }

    std::vector<std::string> Object::getAttributes()
    {
        std::vector<std::string> result;

        for (auto attr : attributes) {
            result.push_back(attr.first);
        }
        return result;
    }

}
}
