#include "ProductionConponent.h"
#include "world/building.h"
#include "StorageComponent.h"
#include "WorkerComponent.h"

namespace world
{
    namespace buildings
    {
        ProductionComponent::ProductionComponent() : BuildingComponent("ProductionComponent")
        {
        }

        ProductionComponent::~ProductionComponent()
        {
        }

        void ProductionComponent::fromJson([[maybe_unused]] std::shared_ptr<utils::JSON::Object> &object, [[maybe_unused]] Company *company)
        {
        }

        void ProductionComponent::updateProduction(int month, [[maybe_unused]] int year, Building *building)
        {
            auto storage = building->getComponent<world::buildings::StorageComponent>("StorageComponent");
            auto workerComp = building->getComponent<world::buildings::WorkerComponent>("WorkerComponent");

            int usedWorkers = 0;
            int maxWorkers = workerComp->getCurrentWorkers();

            for (auto &product : building->getProducts())
            {

                auto &cycle = product->getProductionCycle();
                if (static_cast<unsigned>(month) >= cycle.startMonth && static_cast<unsigned>(month) <= cycle.endMonth)
                {
                    if (product->getBaseProducts().size() > 0)
                    {
                        bool requirementsFullfilled = true;
                        for (auto &base : product->getBaseProducts())
                        {
                            int amount = storage->getEntry(base->product->getName());
                            if (amount < base->amount)
                            {
                                requirementsFullfilled = false;
                                break;
                            }
                        }
                        if (requirementsFullfilled)
                        {
                            for (auto &base : product->getBaseProducts())
                            {
                                storage->addEntry(base->product->getName(), base->amount * -1);
                            }
                            storage->addEntry(product->getName(), cycle.amount);
                            usedWorkers++;
                        }
                    }
                    else if (product->getResources().size() > 0)
                    {
                        storage->addEntry(product->getName(), cycle.amount);
                    }

                    if (usedWorkers == maxWorkers)
                    {
                        break;
                    }
                }
            }
        }

        std::shared_ptr<BuildingComponent> ProductionComponent::clone()
        {
            return std::make_shared<ProductionComponent>();
        }

        std::shared_ptr<utils::JSON::Object> ProductionComponent::toJson()
        {
            auto object = world::buildings::BuildingComponent::toJson();
            return object;
        }

        std::map<std::string, std::string> ProductionComponent::displayData()
        {
            std::map<std::string, std::string> data;

            return data;
        }
    }
}