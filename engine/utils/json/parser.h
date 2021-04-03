#ifndef PARSER_H
#define PARSER_H

#include "../string.h"
#include "object.h"
#include <memory>
#include <string>
#include <vector>
namespace utils {

namespace JSON {

    class Parser {
    public:
        Parser();

        void parseString(const std::string& jsonData);
        std::vector<std::variant<int, float, std::string, std::shared_ptr<Object>>> parseArray(const std::string& jsonData);
        std::shared_ptr<Object> parseObject(const std::string& jsonData);

    private:
        void parseAttribute(const std::string& jsonData);
        size_t findPositionInString(const std::string data, char starChar, char endChar, const size_t startPosition);
    };
}
}

#endif // PARSER_H
