#include "textparser.h"
#include <iostream>

namespace utils {

TextParser::TextParser()
{
}
void TextParser::createValueElement(std::string text)
{
    TextElement value;
    value.type = TextElementType::Text;
    value.elementValue = text;
    elementList.push_back(value);
}
void TextParser::createTagElement(std::string text)
{
    TextElement tag;
    tag.type = TextElementType::Tag;
    tag.elementName = text;
    elementList.push_back(tag);
}
void TextParser::parseText(std::string text)
{
    size_t lastPos = 0;
    size_t startPos = 0;
    for (size_t pos = 0; pos < text.size(); ++pos) {
        switch (text[pos]) {
        case '[':
            createValueElement(text.substr(lastPos, pos - lastPos));
            startPos = pos;

            break;
        case ']':
            createTagElement(text.substr(startPos + 1, pos - startPos - 1));
            lastPos = pos + 1;
            break;
        }
    }
    createValueElement(text.substr(lastPos));
}
std::vector<TextElement> TextParser::getElementList()
{
    return elementList;
}
}
