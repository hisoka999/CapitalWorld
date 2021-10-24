#ifndef __BUILDINGSELECTIONCOMPONENT_H__
#define __BUILDINGSELECTIONCOMPONENT_H__
#include "world/building.h"
#include <engine/ui/Container.h>
namespace UI
{

    class BuildingSelectionComponent : public UI::Container, public UI::Object
    {
    public:
        BuildingSelectionComponent(UI::Object *parent, std::shared_ptr<world::Building> &building, const std::shared_ptr<world::Company> &company);
        ~BuildingSelectionComponent();
        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);

    private:
        void initUI();
        std::shared_ptr<world::Building> building;
        std::shared_ptr<world::Company> company;
        bool selected = false;
    };

};
#endif // __BUILDINGSELECTIONCOMPONENT_H__