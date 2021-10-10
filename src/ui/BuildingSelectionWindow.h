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
        void initUI();

    public:
        BuildingSelectionWindow(int x, int y, const std::shared_ptr<world::Company> &company);
        ~BuildingSelectionWindow();
        void setBuildingType(world::BuildingType buildingType);
    };

};
#endif // __BUILDINGSELECTIONWINDOW_H__