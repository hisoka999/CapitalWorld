#include "textloader.h"
#include <fstream>
#include <engine/utils/exceptions.h>

namespace utils {

TextLoader::TextLoader(std::string file)
{
    std::ifstream stream;
    stream.open(file);
    //stream.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    for( std::string line; getline( stream, line ); )
    {
        //line.replace()
        texts.push_back(line.substr(0,line.size()-1));
    }
    stream.close();
}

std::string TextLoader::operator[](int index)
{
    return texts[index];
}

}
