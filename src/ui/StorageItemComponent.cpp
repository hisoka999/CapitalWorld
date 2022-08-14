#include "StorageItemComponent.h"
#include <engine/ui/layout/GridLayout.h>
#include <unordered_map>
#include <engine/ui/Label.h>
#include "world/company.h"
#include "translate.h"
namespace UI
{
    StorageItemComponent::StorageItemComponent(UI::Object *parent)
        : UI::Object(parent), UI::Container()
    {
        initUI();
    }

    StorageItemComponent::~StorageItemComponent()
    {
    }

    void StorageItemComponent::setProduct(const std::shared_ptr<Product> &product)
    {
        m_product = product;
        updateUI();
    }

    void StorageItemComponent::setAmount(int amount)
    {
        m_amount = amount;
        updateUI();
    }

    void StorageItemComponent::render(core::Renderer *pRender)
    {
        UI::Container::render(pRender);
    }

    bool StorageItemComponent::handleEvents(core::Input *pInput)
    {
        return UI::Container::handleEvents(pInput);
    }

    void StorageItemComponent::initUI()
    {
        setLayout(std::make_shared<UI::layout::GridLayout>(this));
    }

    void StorageItemComponent::updateUI()
    {
        clear();
        if (m_product)
        {

            std::vector<KeyValue> displayData;
            displayData.push_back({_("Productname:"), m_product->getLocalisedName()});
            displayData.push_back({_("Production costs per Item:"), format_currency(m_product->calculateCostsPerPiece())});
            displayData.push_back({_("Start month:"), std::to_string(m_product->getProductionCycle().startMonth)});
            displayData.push_back({_("End month:"), std::to_string(m_product->getProductionCycle().endMonth)});
            displayData.push_back({_("Production time:"), std::to_string(m_product->getProductionCycle().productionTime)});
            displayData.push_back({_("Production amount:"), std::to_string(m_product->getProductionCycle().amount)});
            displayData.push_back({_("Storage amount:"), std::to_string(m_amount)});

            for (auto &data : displayData)
            {
                auto nameLabel = std::make_shared<UI::Label>(data.key, this);
                addObject(nameLabel);
                auto valueLabel = std::make_shared<UI::Label>(data.value, this);
                addObject(valueLabel);
            }
        }
        needsRefresh();
    }
}