#include "productcomponent.h"
#include <engine/utils/os.h>

namespace UI
{

    ProductComponent::ProductComponent(std::shared_ptr<Product> product, UI::Object *parent) : product(product), UI::Object(parent)
    {
        image = std::make_shared<UI::ImageButton>(this, 100, 100, 0, 0, true);
        image->loadImage(utils::os::combine("images", "products", product->getImage()));
        addObject(image);

        label = std::make_shared<UI::Label>(product->getName(), this);
        label->setPos(0, 100);
        addObject(label);

        image->connect("buttonClick", [this]() {
            this->fireFuncionCall("imageClicked");
        });
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
