#include "resourceproductiontab.h"
#include "services/ressourceservice.h"
#include <engine/utils/os.h>
#include "translate.h"

namespace UI
{

    ResourceProductionTab::ResourceProductionTab(UI::Object *parent, std::shared_ptr<world::Building> building, world::RawResource rawResource)
        : UI::Tab(parent, _("Production")), rawResource(rawResource), building(building)
    {
        initUI();
        setBuilding(building);
    }
    void ResourceProductionTab::initUI()
    {
        productSelectionBox = std::make_shared<UI::ComboBox<std::shared_ptr<Product>>>(this);
        productSelectionBox->setPos(490, 28);
        productSelectionBox->setWidth(120);

        productSelectionBox->connect("selectionChanged", [&](unsigned int selection)
                                     { productSelectionChanged(selection); });

        productSelectionBox->setElementFunction([&](std::shared_ptr<Product> var)
                                                { return var->getLocalisedName(); });

        addObject(productSelectionBox);

        resourceSelectionBox = std::make_shared<UI::ComboBox<std::shared_ptr<Resource>>>(this);
        resourceSelectionBox->setPos(180, 28);
        resourceSelectionBox->setWidth(120);

        resourceSelectionBox->connect("selectionChanged", [&](unsigned int selection)
                                      { resourceSelectionChanged(selection); });

        if (building->requireResource(rawResource))
        {
            resourceList = services::ResourceService::Instance().getResourcesByBuildingType(world::BuildingType::Resource, rawResource);
        }
        else
        {
            resourceList.clear();
        }
        for (auto &res : resourceList)
        {
            resourceSelectionBox->addElement(res);
            int textHeight, textWidth;
            resourceSelectionBox->getFont()->size(res->getName(), &textHeight, &textWidth);
            if (textWidth > resourceSelectionBox->getWidth())
            {
                resourceSelectionBox->setWidth(textWidth + 20);
            }
        }
        resourceSelectionBox->setElementFunction([&](std::shared_ptr<Resource> var)
                                                 { return var->getLocalisedName(); });

        addObject(resourceSelectionBox);

        resourceImage = std::make_shared<UI::ImageButton>(this, 100, 100, 0, 0, true);
        resourceImage->setPos(180, 70);
        addObject(resourceImage);

        resourceNameText = std::make_shared<UI::Label>(this);
        resourceNameText->setPos(180, 180);
        addObject(resourceNameText);

        productImage = std::make_shared<UI::ImageButton>(this, 100, 100, 0, 0, true);
        productImage->setPos(490, 70);
        addObject(productImage);

        std::shared_ptr<UI::Label> labelCycle = std::make_shared<UI::Label>(_("Production Cycle:"), this);
        labelCycle->setPos(295, 70);
        addObject(labelCycle);

        std::shared_ptr<UI::Label> labelLandType = std::make_shared<UI::Label>(_("Land Type:"), this);
        labelLandType->setPos(295, 100);
        addObject(labelLandType);

        std::shared_ptr<UI::Label> labelCosts = std::make_shared<UI::Label>(_("Costs:"), this);
        labelCosts->setPos(295, 130);
        addObject(labelCosts);

        productNameText = std::make_shared<UI::Label>(this);
        productNameText->setPos(490, 180);
        addObject(productNameText);

        costsText = std::make_shared<UI::Label>(this);
        costsText->setPos(400, 130);
        addObject(costsText);

        productionCycleText = std::make_shared<UI::Label>(this);
        productionCycleText->setPos(400, 70);
        addObject(productionCycleText);

        addButton = std::make_shared<UI::Button>(this);
        addButton->setLabel(_("Add"));
        addButton->setStaticWidth(90);
        addButton->setPos(180, 380);
        addButton->connect(UI::Button::buttonClickCallback(), [&]()
                           {
                               auto product = productList[static_cast<size_t>(productSelectionBox->getSelection())];
                               if (!building->hasProduct(product))
                               {
                                   building->addProduct(product);
                               }
                               else
                               {
                                   building->removeProduct(product);
                               }
                               refreshProductList(); });
        addObject(addButton);

        helpButton = std::make_shared<UI::Button>(this);
        helpButton->setLabel(_("Help"));
        helpButton->setStaticWidth(90);
        helpButton->setPos(180, 420);
        helpButton->disable();
        addObject(helpButton);
        resourceSelectionChanged(0);
        productSelectionChanged(0);
        refreshProductList();
    }

    void ResourceProductionTab::refreshProductList()
    {
        for (auto p : productComponents)
        {
            removeObject(p);
        }
        productComponents.clear();
        if (building == nullptr)
            return;
        int y = 50;
        for (auto product : building->getProducts())
        {
            std::shared_ptr<UI::ProductComponent> pc = std::make_shared<UI::ProductComponent>(product, this);
            pc->connect("imageClicked", [=](void)
                        {
                            std::cout << "click: " << product->getName() << std::endl;
                            resourceSelectionBox->setSelectionByText(product->getResources().at(0)->resource);
                            productSelectionBox->setSelectionByText(product); });

            productComponents.push_back(pc);
            pc->setPos(20, y);
            addObject(pc);
            y += 120;
        }

        if (building != nullptr)
        {
            std::shared_ptr<Product> product = nullptr;
            if (productList.size() > 0)
            {
                product = productList[static_cast<size_t>(productSelectionBox->getSelection())];
            }
            if (product && !building->hasProduct(product))
            {
                addButton->setLabel(_("Add"));
            }
            else
            {
                addButton->setLabel(_("Remove"));
            }
        }
    }

    void ResourceProductionTab::setBuilding(std::shared_ptr<world::Building> building)
    {
        this->building = building;
        refreshProductList();
    }

    void ResourceProductionTab::resourceSelectionChanged(unsigned int selection)
    {
        if (resourceList.size() == 0)
        {
            std::cout << "no resource available" << std::endl;
            return;
        }
        auto resource = resourceList[selection];
        resourceNameText->setText(resource->getLocalisedName());
        costsText->setText(utils::string_format("%'.2f €/m", resource->getCostPerMonth()));
        resourceImage->loadImage(utils::os::combine("images", "products", resource->getImage()));

        // reload products
        productList = services::ProductService::Instance().getProductsByTypeAndResource(world::BuildingType::Resource, resource);
        productSelectionBox->clearElements();
        for (auto &product : productList)
        {
            productSelectionBox->addElement(product);
        }
        productSelectionChanged(0);
    }
    void ResourceProductionTab::productSelectionChanged(unsigned int selection)
    {
        if (productList.size() == 0)
        {
            std::cout << "no products available" << std::endl;
            return;
        }
        auto product = productList[selection];
        productNameText->setText(product->getLocalisedName());
        productImage->loadImage(utils::os::combine("images", "products", product->getImage()));

        productionCycleText->setTextF("%'d  - %'d", product->getProductionCycle().startMonth, product->getProductionCycle().endMonth);

        if (building != nullptr)
        {
            if (!building->hasProduct(product))
            {
                addButton->setLabel(_("Add"));
            }
            else
            {
                addButton->setLabel(_("Remove"));
            }
        }
    }

}
