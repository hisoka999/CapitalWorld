#include "WorkerComponent.h"
#include "translate.h"
#include "world/building.h"
#include <engine/utils/string.h>

namespace world
{
    namespace buildings
    {
        WorkerComponent::WorkerComponent() : world::buildings::BuildingComponent("WorkerComponent")
        {
        }

        WorkerComponent::~WorkerComponent()
        {
        }

        void WorkerComponent::fromJson(std::shared_ptr<utils::JSON::Object> &object, Company *company)
        {
        }

        void WorkerComponent::updateProduction(int month, int year, Building *building)
        {
            int costs = salary * currentWorkers;
            if (costs > 0)
                building->addCosts(month, year, "", world::BalanceAccount::Production, costs);
        }

        std::shared_ptr<BuildingComponent> WorkerComponent::clone()
        {
            auto component = std::make_shared<WorkerComponent>();
            component->salary = salary;
            component->maxWorkers = maxWorkers;
            return component;
        }

        std::shared_ptr<utils::JSON::Object> WorkerComponent::toJson()
        {
        }

        std::map<std::string, std::string> WorkerComponent::displayData()
        {
            std::map<std::string, std::string> result;
            result[_("Workers:")] = std::to_string(currentWorkers) + "/" + std::to_string(maxWorkers);
            result[_("Salary:")] = utils::string_format(u8"%d €", salary);
            return result;
        }

        void WorkerComponent::setMetaData(std::shared_ptr<utils::JSON::Object> jsonData)
        {
            maxWorkers = jsonData->getIntValue("max_worker");
            salary = jsonData->getIntValue("salary");
        }

        int WorkerComponent::getCurrentWorkers()
        {
            return currentWorkers;
        }

        int WorkerComponent::getMaxWorkers()
        {
            return maxWorkers;
        }

        void WorkerComponent::setCurrentWorkers(int workers)
        {
            currentWorkers = workers;
        }
    }
}