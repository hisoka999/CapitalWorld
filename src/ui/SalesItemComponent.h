#ifndef __SALESITEMCOMPONENT_H__
#define __SALESITEMCOMPONENT_H__

#include <engine/ui/Container.h>
#include <engine/ui/Object.h>
#include <engine/ui/Button.h>
#include <world/buildings/SalesComponent.h>
#include <world/building.h>
#include <engine/ui/NumberTextItem.h>

namespace UI
{
    class SalesItemComponent : public UI::Object, public UI::Container
    {
    private:
        std::shared_ptr<world::buildings::SalesItem> salesItem;

    public:
        SalesItemComponent(UI::Object *parent, const std::shared_ptr<world::buildings::SalesItem> &salesItem, std::shared_ptr<world::Building> &building);
        ~SalesItemComponent();
        virtual void render(core::Renderer *pRender);
        virtual bool handleEvents(core::Input *pInput);

        std::shared_ptr<UI::Button> closeButton;
        std::shared_ptr<UI::NumberTextItem> priceField;
    };

} // namespace UI

#endif // __SALESITEMCOMPONENT_H__