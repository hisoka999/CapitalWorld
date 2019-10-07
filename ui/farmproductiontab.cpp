#include "farmproductiontab.h"


namespace UI {

FarmProductionTab::FarmProductionTab(UI::Object* parent)
    : UI::Tab(parent,"Production")
{
    initUI();
}
void FarmProductionTab::initUI()
{
    productSelectionBox = std::make_shared<UI::ComboBox>(this);
    productSelectionBox->setPos(180,28);
    auto products = services::ProductService::Instance().getProductsByBuildingType(BuildingType::Farm);
    for(auto& product: products)
    {
        productSelectionBox->addElement(product.getName());
    }


    addObject(productSelectionBox.get());

    productImage = std::make_shared<UI::ImageButton>(this,100,100,0,0,true);
    productImage->setPos(180,70);
    addObject(productImage.get());

    UI::Label *labelCycle = new UI::Label("Production Cycle:",this);
    labelCycle->setPos(330,70);
    addObject(labelCycle);

    UI::Label *labelLandType = new UI::Label("Land Type:",this);
    labelLandType->setPos(330,100);
    addObject(labelLandType);

    UI::Label *labelCosts = new UI::Label("Costs:",this);
    labelCosts->setPos(330,130);
    addObject(labelCosts);

    productNameText = std::make_shared<UI::Label>(this);
    productNameText->setPos(180,180);
    addObject(productNameText.get());
}

}
