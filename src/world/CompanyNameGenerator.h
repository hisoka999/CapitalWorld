#pragma once

#include <string>
#include <vector>
#include <random>

namespace world
{
#pragma once
    class CompanyNameGenerator
    {
    public:
        CompanyNameGenerator(const std::string &fileName, size_t seed);
        virtual ~CompanyNameGenerator();

        std::string generateName();

    private:
        std::mt19937 m_gen;
        std::vector<std::string> m_topics;
        std::vector<std::string> m_name_helper;
        std::vector<std::string> m_type;
    };
} // namespace world
