#ifndef __BUILDINGSELECTIONWINDOW_H__
#define __BUILDINGSELECTIONWINDOW_H__
#include "world/buildingtypes.h"
#include "world/company.h"
#include <engine/ui/Window.h>
#include <engine/ui/scrollarea.h>
namespace UI
{

    class BuildingSelectionWindow : public UI::Window
    {
    private:
        std::shared_ptr<UI::ScrollArea> scrollArea;
        world::BuildingType buildingType;
        std::shared_ptr<world::Company> company;
        std::shared_ptr<world::Building> selectedBuilding;
        void initUI();

    public:
        BuildingSelectionWindow(int x, int y, const std::shared_ptr<world::Company> &company);
        ~BuildingSelectionWindow();
        void setBuildingType(world::BuildingType buildingType);
        std::shared_ptr<world::Building> getSelectedBuilding();
        void setSelectedBuilding(std::shared_ptr<world::Building> building);
    };

};
#endif // __BUILDINGSELECTIONWINDOW_H__