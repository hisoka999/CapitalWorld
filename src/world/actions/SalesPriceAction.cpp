#include "SalesPriceAction.h"
#include "world/buildings/StorageComponent.h"
#include "world/buildings/SalesComponent.h"
#include "services/productservice.h"
namespace world
{
    namespace actions
    {
        SalesPriceAction::SalesPriceAction(const std::shared_ptr<world::Company> &company)
            : Action(company)
        {
        }

        SalesPriceAction::~SalesPriceAction()
        {
        }

        void SalesPriceAction::execute([[maybe_unused]] const std::shared_ptr<world::GameState> &gameState)
        {
            for (auto &building : m_company->getBuildings())
            {
                if (building->getType() == world::BuildingType::Shop)
                {
                    auto storage = building->getComponent<world::buildings::StorageComponent>("StorageComponent");
                    auto sales = building->getComponent<world::buildings::SalesComponent>("SalesComponent");

                    for (auto &item : sales->getSalesItems())
                    {
                        if (item->active)
                        {
                            auto product = services::ProductService::Instance().getProductByName(item->product);
                            int entry = storage->getEntry(item->product);
                            if (entry < 1000)
                            {
                                item->price *= 1.1;
                            }
                            else if (entry > 5000)
                            {
                                item->price *= 0.9;
                                if (item->price < product->calculateCostsPerPiece())
                                {
                                    item->price = product->calculateCostsPerPiece() * 1.1;
                                }
                            }
                        }
                    }
                }
            }
        }

        bool SalesPriceAction::canExecute([[maybe_unused]] const std::shared_ptr<world::GameState> &gameState)
        {
            for (auto &building : m_company->getBuildings())
            {
                if (building->getType() == world::BuildingType::Shop)
                {
                    return true;
                }
            }
            return false;
        }
    }
}