#include "farmproductiontab.h"
#include <engine/utils/os.h>

namespace UI {

FarmProductionTab::FarmProductionTab(UI::Object* parent)
    : UI::Tab(parent,"Production")
{
    initUI();
}
void FarmProductionTab::initUI()
{
    productSelectionBox = std::make_shared<UI::ComboBox>(this);
    productSelectionBox->setPos(490,28);
    productList = services::ProductService::Instance().getProductsByBuildingType(BuildingType::Farm);
    productSelectionBox->connect("selectionChanged",[&](unsigned int selection){
        productSelectionChanged(selection);
    });

    for(auto& product: productList)
    {
        productSelectionBox->addElement(product->getName());

    }
    addObject(productSelectionBox.get());

    resourceSelectionBox = std::make_shared<UI::ComboBox>(this);
    resourceSelectionBox->setPos(180,28);
    resourceSelectionBox->connect("selectionChanged",[&](unsigned int selection){
        resourceSelectionChanged(selection);
    });

    resourceList = services::ProductService::Instance().getResourcesByBuildingType(BuildingType::Farm);
    for(auto& res : resourceList)
    {
        resourceSelectionBox->addElement(res->getName());
    }


    addObject(resourceSelectionBox.get());

    resourceImage = std::make_shared<UI::ImageButton>(this,100,100,0,0,true);
    resourceImage->setPos(180,70);
    addObject(resourceImage.get());

    resourceNameText = std::make_shared<UI::Label>(this);
    resourceNameText->setPos(180,180);
    addObject(resourceNameText.get());


    productImage = std::make_shared<UI::ImageButton>(this,100,100,0,0,true);
    productImage->setPos(490,70);
    addObject(productImage.get());

    UI::Label *labelCycle = new UI::Label("Production Cycle:",this);
    labelCycle->setPos(295,70);
    addObject(labelCycle);

    UI::Label *labelLandType = new UI::Label("Land Type:",this);
    labelLandType->setPos(295,100);
    addObject(labelLandType);

    UI::Label *labelCosts = new UI::Label("Costs:",this);
    labelCosts->setPos(295,130);
    addObject(labelCosts);

    productNameText = std::make_shared<UI::Label>(this);
    productNameText->setPos(490,180);
    addObject(productNameText.get());

    costsText = std::make_shared<UI::Label>(this);
    costsText->setPos(400,130);
    addObject(costsText.get());

    productionCycleText = std::make_shared<UI::Label>(this);
    productionCycleText->setPos(400,70);
    addObject(productionCycleText.get());

    addButton = std::make_shared<UI::Button>(this);
    addButton->setLabel("Add");
    addButton->setStaticWidth(90);
    addButton->setPos(180,280);
    addButton->connect(UI::Button::buttonClickCallback(),[&](){
        auto product = productList[static_cast<size_t>(productSelectionBox->getSelection())];
        if(!building->hasProduct(product))
        {
            building->addProduct(product);
            //TODO refresh product list
        }
    });
    addObject(addButton.get());


    helpButton = std::make_shared<UI::Button>(this);
    helpButton->setLabel("Help");
    helpButton->setStaticWidth(90);
    helpButton->setPos(180,320);
    helpButton->disable();
    addObject(helpButton.get());
    resourceSelectionChanged(0);
    productSelectionChanged(0);
}
void FarmProductionTab::setBuilding(std::shared_ptr<world::Building> building)
{
    this->building = building;
}

void FarmProductionTab::resourceSelectionChanged(unsigned int selection)
{
    auto resource = resourceList[selection];
    resourceNameText->setText(resource->getName());
    costsText->setText(utils::string_format("%'.2f €/m",resource->getCostPerMonth()));
    resourceImage->loadImage(utils::os::combine("images","products",resource->getImage()));

    // reload products
    productList = services::ProductService::Instance().getProductsByTypeAndResource(BuildingType::Farm,resource);
    productSelectionBox->clearElements();
    for(auto& product: productList)
    {
        productSelectionBox->addElement(product->getName());

    }
    productSelectionChanged(0);
}
void FarmProductionTab::productSelectionChanged(unsigned int selection)
{
    auto product = productList[selection];
    productNameText->setText(product->getName());
    productImage->loadImage(utils::os::combine("images","products",product->getImage()));

    productionCycleText->setTextF("%'d  - %'d",product->getProductionCycle().startMonth,product->getProductionCycle().endMonth);

}

}
