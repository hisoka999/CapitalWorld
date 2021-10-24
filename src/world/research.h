#ifndef RESEARCH_H
#define RESEARCH_H

#include <memory>
#include <string>
#include <vector>
#include <engine/utils/time/date.h>

class Research
{
public:
    Research(const std::string name, const std::string localizedName, const std::string description, const std::string icon, int costs, utils::time::Date &date);

    std::string getName() const;

    std::string getLocalisedName() const;

    std::string getDescription() const;

    std::string getIcon() const;

    int getCosts() const;

    std::vector<std::shared_ptr<Research>> getRequirements() const;

    bool getResearched() const;

    std::vector<std::string> getRequirementNames() const;
    void setRequirementNames(const std::vector<std::string> &value);

    void setEnabledObjects(const std::vector<std::string> &value);

    void addRequirement(const std::shared_ptr<Research> &requirement);

    bool canEnableObject(const std::string &name);

    bool canResearch(const utils::time::Date &currentDate);

    int getCurrentCosts() const;
    void reduceCosts(int value);

private:
    std::string name;
    std::string localisedName;
    std::string description;
    std::string icon;
    int costs;
    std::vector<std::shared_ptr<Research>> requirements;
    std::vector<std::string> requirementNames;
    bool researched;
    std::vector<std::string> enabledObjects;
    int currentCosts;
    utils::time::Date date;
};

#endif // RESEARCH_H
