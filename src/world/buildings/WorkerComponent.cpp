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

        void WorkerComponent::fromJson(std::shared_ptr<utils::JSON::Object> &object, [[maybe_unused]] Company *company)
        {

            currentWorkers = object->getIntValue("currentWorkers");
        }

        void WorkerComponent::updateProduction(int month, int year, Building *building)
        {
            int costs = salary * currentWorkers;
            if (costs > 0)
                building->getBalance().addCosts(month, year, "", world::BalanceAccount::Production, costs);
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
            auto object = world::buildings::BuildingComponent::toJson();
            object->setAttribute("currentWorkers", currentWorkers);
            return object;
        }

        std::map<std::string, std::string> WorkerComponent::displayData()
        {
            std::map<std::string, std::string> result;
            result[_("Workers:")] = std::to_string(currentWorkers) + "/" + std::to_string(maxWorkers);
            result[_("Salary:")] = format_currency(salary);
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
            if (maxWorkers < workers)
            {
                currentWorkers = maxWorkers;
            }
            else
            {
                currentWorkers = workers;
            }
        }
    }
}