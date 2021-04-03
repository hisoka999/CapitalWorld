#ifndef PRODUCTCOMPONENT_H
#define PRODUCTCOMPONENT_H

#include "../world/product.h"
#include <engine/ui/Container.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>

namespace UI
{

    class ProductComponent : public UI::Container, public UI::Object
    {
    public:
        ProductComponent(std::shared_ptr<Product> product, UI::Object *parent);
        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);

    private:
        std::shared_ptr<Product> product;
        std::shared_ptr<UI::ImageButton> image;
        std::shared_ptr<UI::Label> label;
    };

}
#endif // PRODUCTCOMPONENT_H
