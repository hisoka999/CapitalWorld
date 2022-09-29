#include "SalesItemComponent.h"
#include <engine/ui/Theme.h>
#include <engine/ui/Label.h>
#include <engine/ui/NumberTextItem.h>
#include <engine/ui/ComboBox.h>
#include <world/buildings/StorageComponent.h>
#include <services/productservice.h>
#include <engine/ui/Checkbox.h>
#include "translate.h"
namespace UI
{
    SalesItemComponent::SalesItemComponent(UI::Object *parent, const std::shared_ptr<world::buildings::SalesItem> &salesItem, std::shared_ptr<world::Building> &building) : UI::Object(parent), salesItem(salesItem)
    {
        setWidth(parent->displayRect().width - 10);
        setHeight(100);
        setObjectName("SalesItemComponent");
        closeButton = std::make_shared<UI::Button>(this);
        closeButton->setX(parent->displayRect().width - 50);
        closeButton->setY(10);
        std::string iconFontName = getTheme()->getStyleText(this, UI::StyleType::IconFontName);
        int iconFontSize = getTheme()->getStyleInt(this, UI::StyleType::IconFontSize);

        closeButton->setFont(iconFontName, iconFontSize);
        closeButton->setLabel("\uf00d");
        closeButton->setBorderless(true);

        addObject(closeButton);

        auto nameLabel = std::make_shared<UI::Label>(_("Name: "), this);
        nameLabel->setPos(110, 10);
        nameLabel->setFont("fonts/arial.ttf", 1);
        addObject(nameLabel);

        auto productCombobox = std::make_shared<UI::ComboBox<std::string>>(this);
        const auto &storage = building->getComponent<world::buildings::StorageComponent>("StorageComponent");
        for (auto product : storage->getStoredProducts())
        {
            productCombobox->addElement(product);
        }
        productCombobox->setSelectionByText(salesItem->product);
        productCombobox->setElementFunction([](std::string val)
                                            { return val; });

        priceField = std::make_shared<UI::NumberTextItem>(this, 100, 20);
        priceField->setPos(170, 40);

        priceField->setValue(salesItem->price);
        priceField->connect("valueChanged", [&](float val)
                            { salesItem->price = val; });

        addObject(priceField);

        if (salesItem->product.empty() && storage->getStoredProducts().size() > 0)
        {
            salesItem->product = productCombobox->getSelectionText();
            auto product = services::ProductService::Instance().getProductByName(salesItem->product);
            salesItem->price = product->calculateCostsPerPiece() * 1.5;
            priceField->setValue(salesItem->price);
        }
        productCombobox->connect("valueChanged", [&](std::string value)
                                 {
                                     salesItem->product = value;
                                     auto product = services::ProductService::Instance().getProductByName(value);
                                     salesItem->price = product->calculateCostsPerPiece() * 1.5;
                                     priceField->setValue(salesItem->price); });

        productCombobox->setPos(170, 10);
        productCombobox->setWidth(200);
        addObject(productCombobox);

        auto priceLabel = std::make_shared<UI::Label>(_("Price: "), this);
        priceLabel->setFont("fonts/arial.ttf", 12);
        priceLabel->setPos(110, 40);
        addObject(priceLabel);

        auto activeCheckBox = std::make_shared<UI::Checkbox>(this);
        activeCheckBox->setFont("fonts/arial.ttf", 12);
        activeCheckBox->setText(_("active"));

        activeCheckBox->setPos(380, 10);
        addObject(activeCheckBox);
        activeCheckBox->setChecked(salesItem->active);
        activeCheckBox->connect("checkboxChanged", [&](bool value)
                                { salesItem->active = value; });
    }

    SalesItemComponent::~SalesItemComponent()
    {
    }

    void SalesItemComponent::render(core::Renderer *pRender)
    {
        UI::Container::render(pRender);
    }

    bool SalesItemComponent::handleEvents(core::Input *pInput)
    {
        return UI::Container::handleEvents(pInput);
    }
}