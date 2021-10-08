#ifndef RESEARCHSERVICE_H
#define RESEARCHSERVICE_H

#include "world/research.h"
#include <engine/utils/json/jsonservice.h>
#include <engine/utils/json/parser.h>
#include <memory>
#include <mutex>
#include <vector>

namespace services
{
    std::string utf8chr(int cp);

    class ResearchService : public utils::json::JSONService<Research>
    {
    public:
        static ResearchService &Instance()
        {
            static ResearchService instance;
            return instance;
        }

    protected:
        std::shared_ptr<Research> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object);

    private:
        ResearchService();
        ~ResearchService() = default;
        ResearchService(const ResearchService &) = delete;
        ResearchService &operator=(const ResearchService &) = delete;
    };
};
#endif // RESEARCHSERVICE_H
