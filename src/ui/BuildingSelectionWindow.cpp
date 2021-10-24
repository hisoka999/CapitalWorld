#include "BuildingSelectionWindow.h"
#include "BuildingSelectionComponent.h"
#include "translate.h"
namespace UI
{
    void BuildingSelectionWindow::initUI()
    {
        scrollArea->clear();
        auto buildings = company->findAvailableBuildingsByType(buildingType);

        int yPos = 0;

        for (auto &building : buildings)
        {
            auto comp = std::make_shared<UI::BuildingSelectionComponent>(scrollArea.get(), building, company);
            comp->setPos(0, yPos);
            yPos += 110;
            scrollArea->addObject(comp);
        }
        scrollArea->reset(false);

        scrollArea->connect("selectItem", [&](int selection)
                            {
                                auto buildings = company->findAvailableBuildingsByType(buildingType);

                                selectedBuilding = buildings.at(selection);
                                this->fireFuncionCall("buildingSelectionChanged", selectedBuilding);
                                auto component = std::dynamic_pointer_cast<UI::BuildingSelectionComponent>(scrollArea->get(selection));
                                std::cout << "selected building: " << selectedBuilding->getDisplayName() << std::endl;
                                setVisible(false);
                            });
    }

    BuildingSelectionWindow::BuildingSelectionWindow(int x, int y, const std::shared_ptr<world::Company> &company) : UI::Window(x, y, 300, 400), company(company)
    {
        setTitle(_("select a building"));
        scrollArea = std::make_shared<UI::ScrollArea>(280, 380, this);
        addObject(scrollArea);
        selectedBuilding = nullptr;
    }

    BuildingSelectionWindow::~BuildingSelectionWindow()
    {
    }

    void BuildingSelectionWindow::setBuildingType(world::BuildingType buildingType)
    {
        this->buildingType = buildingType;
        initUI();
    }

    std::shared_ptr<world::Building> BuildingSelectionWindow::getSelectedBuilding()
    {
        return selectedBuilding;
    }

    void BuildingSelectionWindow::setSelectedBuilding(std::shared_ptr<world::Building> building)
    {
        selectedBuilding = building;
        this->fireFuncionCall("buildingSelectionChanged", selectedBuilding);
    }
}