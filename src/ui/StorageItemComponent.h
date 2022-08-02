#pragma once

#include <engine/ui/Object.h>
#include <engine/ui/Container.h>
#include "world/product.h"
namespace UI
{
    class StorageItemComponent : public UI::Object, public UI::Container
    {
    public:
        StorageItemComponent(UI::Object *parent = nullptr);
        ~StorageItemComponent();
        void setProduct(const std::shared_ptr<Product> &product);
        virtual void render(core::Renderer *pRender);
        virtual bool handleEvents(core::Input *pInput);

    private:
        void initUI();
        void updateUI();

        std::shared_ptr<Product> m_product = nullptr;
    };

} // namespace UI
