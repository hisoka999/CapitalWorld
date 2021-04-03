#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <string>
#include <vector>

namespace utils {

enum class TextElementType{
    Text,Tag
};

class TextElement {

public:
    TextElementType type;
    std::string elementName;
    std::string elementValue;
};


class TextParser
{
public:
    TextParser();

    void parseText(std::string text);

    std::vector<TextElement> getElementList();

private:
    void createValueElement(std::string text);
    void createTagElement(std::string text);
    std::vector<TextElement> elementList;

};

}

#endif // TEXTPARSER_H
