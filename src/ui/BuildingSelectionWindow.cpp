#include "BuildingSelectionWindow.h"
#include "BuildingSelectionComponent.h"
namespace UI
{
    void BuildingSelectionWindow::initUI()
    {
        auto buildings = company->findAvailableBuildingsByType(buildingType);

        int yPos = 0;
        for (auto &building : buildings)
        {
            auto comp = std::make_shared<UI::BuildingSelectionComponent>(scrollArea.get(), building);
            comp->setPos(0, yPos);
            yPos += 110;
            scrollArea->addObject(comp);
        }
    }

    BuildingSelectionWindow::BuildingSelectionWindow(int x, int y, const std::shared_ptr<world::Company> &company) : UI::Window(x, y, 200, 300), company(company)
    {
        scrollArea = std::make_shared<UI::ScrollArea>(190, 280, this);
        addObject(scrollArea);
    }

    BuildingSelectionWindow::~BuildingSelectionWindow()
    {
    }

    void BuildingSelectionWindow::setBuildingType(world::BuildingType buildingType)
    {
        this->buildingType = buildingType;
    }
}