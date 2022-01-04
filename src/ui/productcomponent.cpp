#include "productcomponent.h"
#include <engine/utils/os.h>

namespace UI
{

    ProductComponent::ProductComponent(std::shared_ptr<ProduceableObject> product, UI::Object *parent) : UI::Object(parent), product(product)
    {
        image = std::make_shared<UI::ImageButton>(this, 32, 32, 0, 0, true);
        image->loadImage(utils::os::combine("images", "products", product->getImage()));
        addObject(image);

        label = std::make_shared<UI::Label>(product->getLocalisedName(), this);
        label->setPos(0, 40);
        addObject(label);

        image->connect("buttonClick", [this]()
                       { this->fireFuncionCall("imageClicked"); });
    }
    void ProductComponent::render(core::Renderer *pRender)
    {
        UI::Container::render(pRender);
    }
    void ProductComponent::handleEvents(core::Input *pInput)
    {
        UI::Container::handleEvents(pInput);
    }

}
