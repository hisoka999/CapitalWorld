#include "valuetype.h"

namespace utils {

namespace JSON {
    ValueType::ValueType(const int value)
    {
        data = { value };
        type = JsonDataType::IntValue;
    }
    ValueType::ValueType(const float value)
    {
        data = { value };
        type = JsonDataType::FloatValue;
    }
    ValueType::ValueType(const std::string& value)
    {
        data = { value };
        type = JsonDataType::IntValue;
    }
    ValueType::ValueType(const bool value)
    {
        data = { value };
        type = JsonDataType::IntValue;
    }

    ValueType::~ValueType()
    {
    }

}

}
