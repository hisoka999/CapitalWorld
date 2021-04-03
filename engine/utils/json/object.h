#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace utils {
namespace JSON {

    class Object;

    typedef std::variant<int, float, std::string, std::shared_ptr<Object>> JsonValue;
    typedef std::vector<JsonValue> JsonArray;

    class Object {
    public:
        Object();

        template <typename T>
        void setAttribute(const std::string& attr, T value)
        {
            attributes[attr] = value;
        }

        void setArrayAttribute(const std::string& attr, JsonArray array);

        std::string getStringValue(const std::string& attr) const;
        int getIntValue(const std::string& attr) const;
        std::shared_ptr<Object> getObjectValue(const std::string& attr) const;
        JsonArray getArray(const std::string& attr) const;

        std::vector<std::string> getAttributes();

    private:
        std::map<std::string, JsonValue> attributes;
        std::map<std::string, JsonArray> arrayAttributes;
    };
}
}

#endif // OBJECT_H
