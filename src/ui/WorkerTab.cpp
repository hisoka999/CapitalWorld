#include "WorkerTab.h"
#include <engine/ui/ComboBox.h>
#include <engine/ui/Label.h>
#include "world/buildings/WorkerComponent.h"
#include <translate.h>
namespace UI
{
    void WorkerTab::initUI()
    {
        clear();
        auto labelNumberOfWorker = std::make_shared<UI::Label>(_("Workers: "), this);
        labelNumberOfWorker->setPos(5, 5);
        addObject(labelNumberOfWorker);

        auto cbxNumberOfWorker = std::make_shared<UI::ComboBox<int>>(this);
        auto component = building->getComponent<world::buildings::WorkerComponent>("WorkerComponent");

        for (int i = 0; i <= component->getMaxWorkers(); ++i)
        {
            cbxNumberOfWorker->addElement(i);
        }
        cbxNumberOfWorker->setPos(100, 5);
        cbxNumberOfWorker->setElementFunction([](int val)
                                              { return std::to_string(val); });
        cbxNumberOfWorker->setSelectionByText(component->getCurrentWorkers());

        cbxNumberOfWorker->connect("valueChanged", [&](int value)
                                   {
                                       //
                                       auto component2 = building->getComponent<world::buildings::WorkerComponent>("WorkerComponent");
                                       component2->setCurrentWorkers(value); });

        addObject(cbxNumberOfWorker);
    }

    WorkerTab::WorkerTab(UI::Object *parent, const std::shared_ptr<world::Building> &building)
        : UI::Tab(parent, _("Workers"))
    {
        setBuilding(building);
    }

    WorkerTab::~WorkerTab()
    {
    }

    void WorkerTab::setBuilding(const std::shared_ptr<world::Building> &building)
    {
        this->building = building;
        initUI();
    }
}