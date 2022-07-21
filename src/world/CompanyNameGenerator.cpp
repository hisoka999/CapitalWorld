#include "CompanyNameGenerator.h"
#include <engine/utils/exceptions.h>
#include <engine/utils/json/parser.h>
#include <engine/utils/string.h>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>
#include <random>

namespace world
{
    CompanyNameGenerator::CompanyNameGenerator(const std::string &fileName, size_t seed) : m_gen(seed)
    {
        utils::JSON::Parser parser;

        std::ifstream file;
        std::istringstream is;
        std::string s;
        std::string group;

        file.open(fileName.c_str(), std::ios::in);
        if (!file.is_open())
        {
            throw IOException(fileName, "file does not exists");
        }
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        std::string buffer(size, ' ');
        file.seekg(0);
        file.read(&buffer[0], size);

        auto data = parser.parseObject(buffer);

        for (auto tmp : data->getArray("topics"))
            m_topics.push_back(std::get<std::string>(tmp));

        for (auto tmp : data->getArray("name_helper"))
            m_name_helper.push_back(std::get<std::string>(tmp));

        for (auto tmp : data->getArray("type"))
            m_type.push_back(std::get<std::string>(tmp));
    }

    CompanyNameGenerator::~CompanyNameGenerator()
    {
    }

    std::string CompanyNameGenerator::generateName()
    {
        std::uniform_int_distribution<long> nameTopic(0, m_topics.size() - 1);
        std::uniform_int_distribution<long> nameHelper(0, m_name_helper.size() - 1);
        std::uniform_int_distribution<long> nameType(0, m_type.size() - 1);

        std::string name = m_topics[nameTopic(m_gen)];
        name += " " + m_name_helper[nameHelper(m_gen)];
        name += " " + m_type[nameType(m_gen)];
        name = utils::trim(name, " ");
        return name;
    }
}