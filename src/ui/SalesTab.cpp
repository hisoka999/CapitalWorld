#include "SalesTab.h"
#include "world/buildings/SalesComponent.h"
#include <engine/ui/Button.h>
#include "translate.h"
#include <ui/SalesItemComponent.h>
namespace UI
{

    SalesTab::SalesTab(UI::Object *parent, const std::shared_ptr<world::Building> &building) : UI::Tab(parent, "Sales")
    {

        scrollArea = std::make_shared<UI::ScrollArea>(parent->getWidth() - 100, parent->getHeight() - 50, this);
        addObject(scrollArea);

        auto addButton = std::make_shared<UI::Button>(this);
        addButton->setLabel(_("Add Product to sell"));
        addButton->setPos(10, parent->getHeight() - 50);
        addButton->connect(UI::Button::buttonClickCallback(), [&]()
                           {
                               auto component = this->building->getComponent<world::buildings::SalesComponent>("SalesComponent");
                               component->addSalesItem("", 0);
                               refreshUI();
                           });
        addObject(addButton);

        setBuilding(building);
    }

    SalesTab::~SalesTab()
    {
    }

    void SalesTab::refreshUI()
    {
        auto component = building->getComponent<world::buildings::SalesComponent>("SalesComponent");
        scrollArea->clear();

        int yOffset = 0;
        size_t i = 0;
        for (auto &salesItem : component->getSalesItems())
        {
            std::shared_ptr<UI::SalesItemComponent> itemComponent = std::make_shared<SalesItemComponent>(scrollArea.get(), salesItem, building);
            itemComponent->setPos(0, yOffset);
            scrollArea->addObject(itemComponent);
            const size_t value = i;
            itemComponent->closeButton->connect(UI::Button::buttonClickCallback(), [=]()
                                                {
                                                    component->removeSalesItem(value);
                                                    needsRefresh();
                                                });

            yOffset += 100;
            i++;
        }

        scrollArea->reset(false);
    }

    void SalesTab::setBuilding(const std::shared_ptr<world::Building> &building)
    {
        this->building = building;
        refreshUI();
    }

    void SalesTab::refresh()
    {
        refreshUI();
    }
}
