#include "ResearchAction.h"
#include "world/buildings/WorkerComponent.h"

namespace world
{
    namespace actions
    {
        ResearchAction::ResearchAction(const std::shared_ptr<world::Company> &company, const std::shared_ptr<world::Building> &building)
            : Action(company), m_building(building)
        {
        }

        void ResearchAction::execute([[maybe_unused]] const std::shared_ptr<world::GameState> &gameState)
        {

            auto &avialableResearch = m_company->getAvailableResearch();

            auto workers = m_building->getComponent<buildings::WorkerComponent>("WorkerComponent");

            if (avialableResearch.empty())
            {
                workers->setCurrentWorkers(0);
                return;
            }
            if (m_company->getMonthlyProfit() > 0)
            {
                if (workers->getCurrentWorkers() + 1 < workers->getMaxWorkers())
                    workers->setCurrentWorkers(workers->getCurrentWorkers() + 1);
            }
            else if (workers->getCurrentWorkers() > 0)
            {
                workers->setCurrentWorkers(workers->getCurrentWorkers() - 1);
            }

            if (workers->getCurrentWorkers() > 0)
            {

                if (!avialableResearch.empty())
                {
                    m_company->addResearchToQueue(avialableResearch.front());
                }
            }
        }

        bool ResearchAction::canExecute([[maybe_unused]] const std::shared_ptr<world::GameState> &gameState)
        {
            if (!m_company->getResearchQueue().empty())
            {
                return false;
            }
            auto &avialableResearch = m_company->getAvailableResearch();
            auto workers = m_building->getComponent<buildings::WorkerComponent>("WorkerComponent");

            if (avialableResearch.empty() && workers->getCurrentWorkers() == 0)
                return false;

            return true;
        }

    }
}