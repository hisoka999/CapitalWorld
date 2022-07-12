#include "BuildingSelectionWindow.h"
#include "ui/BuildingSelectionComponent.h"
#include "translate.h"
namespace UI
{
    void BuildingSelectionWindow::initUI()
    {
        auto buildings = company->findAvailableBuildingsByType(buildingType);

        int yPos = 0;
        clear();

        for (auto &building : buildings)
        {
            auto comp = std::make_shared<UI::BuildingSelectionComponent>(this, building, company);
            comp->setPos(0, yPos);
            yPos += 110;

            addObject(comp);
            comp->connect("clicked", [&](std::shared_ptr<world::Building> _building)
                          {
                
                setSelectedBuilding(_building);
                setVisible(false); });
        }
    }

    BuildingSelectionWindow::BuildingSelectionWindow(int x, int y, const std::shared_ptr<world::Company> &company) : UI::Window(x, y, 300, 600), company(company)
    {
        setTitle(_("select a building"));
        setObjectName("BuildingSelectionWindow");

        selectedBuilding = nullptr;
    }

    BuildingSelectionWindow::~BuildingSelectionWindow()
    {
    }

    void BuildingSelectionWindow::setBuildingType(world::BuildingType buildingType)
    {
        this->buildingType = buildingType;
        setSelectedBuilding(nullptr);
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