#ifndef VALUETYPE_H
#define VALUETYPE_H

#include <cstdint>
#include <string>
#include <variant>

namespace utils {
namespace JSON {

    enum class JsonDataType {
        NullValue,
        BooleanValue,
        IntValue,
        FloatValue,
        StringValue,
        Array

    };

    class ValueType {
    public:
        ValueType(const int value);
        ValueType(const float value);
        ValueType(const std::string& value);
        ValueType(const bool value);
        virtual ~ValueType();

    private:
        std::variant<int, float, std::string> data;
        JsonDataType type;
    };
}
}
#endif // VALUETYPE_H
