#include "researchservice.h"
#include <algorithm>
#include <engine/utils/localisation.h>
#include <engine/utils/string.h>
#include <engine/utils/time/date.h>
#include <fstream>
#include <magic_enum.hpp>

namespace services
{

    ResearchService::ResearchService()
    {
    }

    std::shared_ptr<Research> ResearchService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
    {

        std::string lang = Localisation::Instance().getLanguage();
        if (lang == "en")
            lang = "";
        else
        {
            lang = "_" + lang;
        }
        std::string name = object->getStringValue("name");
        std::string icon = object->getStringValue("icon");
        std::istringstream hex_chars_stream(icon.substr(2));

        int c;

        std::string icon2;
        std::string data = icon.substr(2);
        hex_chars_stream >> std::hex >> c;
        icon2 = utils::utf8chr(c);

        std::string test = "\uf197";

        std::string localisedName = object->getStringValue("name" + lang);

        std::string description = object->getStringValue("description");
        if (object->hasAttribute("description" + lang))
        {
            description = object->getStringValue("description" + lang);
        }

        auto startDateObj = object->getObjectValue("start_date");

        utils::time::Date startDate(startDateObj->getIntValue("year"), startDateObj->getIntValue("month"), 1);

        int costsPerMonth = object->getIntValue("costs");

        std::shared_ptr<Research> research = std::make_shared<Research>(name, localisedName, description, icon2, costsPerMonth, startDate);

        auto enabledObjs = object->getArray("enables");
        auto requirements = object->getArray("requires");
        std::vector<std::string> enabledObjectNames;
        std::vector<std::string> requirementNames;
        for (auto obj : enabledObjs)
        {
            auto objName = std::get<std::string>(obj);
            enabledObjectNames.push_back(objName);
        }

        for (auto obj : requirements)
        {
            auto objName = std::get<std::string>(obj);
            requirementNames.push_back(objName);
        }
        research->setEnabledObjects(enabledObjectNames);
        research->setRequirementNames(requirementNames);

        return research;
    }
}
