#ifndef TEXTLOADER_H
#define TEXTLOADER_H

#include <vector>
#include <string>

namespace utils {

class TextLoader
{
public:
    TextLoader(std::string path);

    std::vector<std::string> texts;
    std::string operator[](int index);
};
}

#endif // TEXTLOADER_H
