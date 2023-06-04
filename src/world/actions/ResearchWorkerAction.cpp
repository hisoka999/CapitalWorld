#include "ResearchWorkerAction.h"
#include "world/buildings/WorkerComponent.h"
namespace world::actions
{
    ResearchWorkerAction::ResearchWorkerAction(const std::shared_ptr<world::Company> &company)
        : Action(company)
    {
    }

    bool ResearchWorkerAction::canExecute([[maybe_unused]] const std::shared_ptr<world::GameState> &gameState)
    {
        return !m_company->findBuildingsByType(world::BuildingType::Other).empty();
    }

    void ResearchWorkerAction::execute([[maybe_unused]] const std::shared_ptr<world::GameState> &gameState)
    {
        int profit = m_company->getYearlyProfit(gameState->getTime().getYear() - 1);
        for (auto &building : m_company->findBuildingsByType(world::BuildingType::Other))
        {
            if (building->hasComponent("WorkerComponent"))
            {
                auto workers = building->getComponent<world::buildings::WorkerComponent>("WorkerComponent");

                if (profit < workers->getSalary())
                {
                    if (workers->getCurrentWorkers() > 0)
                    {
                        workers->setCurrentWorkers(workers->getCurrentWorkers() - 1);
                        profit += workers->getSalary();
                        break;
                    }
                }
                else
                {
                    workers->setCurrentWorkers(workers->getCurrentWorkers() + 1);
                    profit -= workers->getSalary();
                    break;
                }
            }
        }
    }
} // namespace actions
