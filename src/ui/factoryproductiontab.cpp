#include "factoryproductiontab.h"

#include <engine/utils/os.h>
#include "translate.h"

namespace UI
{

    FactoryProductionTab::FactoryProductionTab(UI::Object *parent, std::shared_ptr<world::Building> building, const std::shared_ptr<world::Company> &player)
        : UI::Tab(parent, _("Production")), player(player)
    {
        initUI();
        setBuilding(building);
    }
    void FactoryProductionTab::initUI()
    {
        productSelectionBox = std::make_shared<UI::ComboBox<std::shared_ptr<Product>>>(this);
        productSelectionBox->setPos(490, 28);
        productSelectionBox->setWidth(100);
        productList = services::ProductService::Instance().getProductsByBuildingType(world::BuildingType::Factory);
        productSelectionBox->connect("selectionChanged", [&](unsigned int selection)
                                     { productSelectionChanged(selection); });

        for (auto &product : productList)
        {
            productSelectionBox->addElement(product);
        }
        productSelectionBox->setElementFunction([&](std::shared_ptr<Product> var)
                                                { return var->getLocalisedName(); });

        addObject(productSelectionBox);

        resourceSelectionBox = std::make_shared<UI::ComboBox<std::shared_ptr<Product>>>(this);
        resourceSelectionBox->setPos(180, 28);
        resourceSelectionBox->setWidth(100);
        resourceSelectionBox->connect("selectionChanged", [&](unsigned int selection)
                                      { resourceSelectionChanged(selection); });

        resourceSelectionBox->setElementFunction([&](std::shared_ptr<Product> var)
                                                 { return var->getLocalisedName(); });

        baseProductList = player->findAvialableBaseProducts(world::BuildingType::Factory);
        for (auto &res : baseProductList)
        {
            resourceSelectionBox->addElement(res);
        }

        addObject(resourceSelectionBox);

        productImage = std::make_shared<UI::ImageButton>(this, 32, 32, 0, 0, true);
        productImage->setPos(490, 70);
        addObject(productImage);

        std::shared_ptr<UI::Label> labelCycle = std::make_shared<UI::Label>(_("Production Cycle:"), this);
        labelCycle->setPos(295, 70);
        addObject(labelCycle);

        std::shared_ptr<UI::Label> labelCosts = std::make_shared<UI::Label>(_("Costs:"), this);
        labelCosts->setPos(295, 100);
        addObject(labelCosts);

        std::shared_ptr<UI::Label> labelCostsPerPerPiece = std::make_shared<UI::Label>(_("Costs per Piece:"), this);
        labelCostsPerPerPiece->setPos(295, 130);

        productNameText = std::make_shared<UI::Label>(this);
        productNameText->setPos(490, 110);
        addObject(productNameText);

        costsText = std::make_shared<UI::Label>(this);
        costsText->setPos(400, 100);
        addObject(costsText);

        costsPerPieceText = std::make_shared<UI::Label>("0,00 €", this);
        costsPerPieceText->setPos(400, 130);
        addObject(costsPerPieceText);

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

    void FactoryProductionTab::refreshProductList()
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
                            resourceSelectionBox->setSelectionByText(product->getBaseProducts().at(0)->product);
                            productSelectionBox->setSelectionByText(product); });

            productComponents.push_back(pc);
            pc->setPos(20, y);
            addObject(pc);
            y += 120;
        }

        if (building != nullptr)
        {
            auto product = productList[static_cast<size_t>(productSelectionBox->getSelection())];
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

    void FactoryProductionTab::setBuilding(std::shared_ptr<world::Building> building)
    {
        this->building = building;
        refreshProductList();
    }

    void FactoryProductionTab::resourceSelectionChanged(unsigned int selection)
    {
        if (baseProductList.size() == 0)
            return;
        auto resource = baseProductList[selection];

        // reload products
        productList = services::ProductService::Instance().getProductsByBuildingType(world::BuildingType::Factory, resource);
        productSelectionBox->clearElements();
        for (auto &product : productList)
        {
            productSelectionBox->addElement(product);
        }
        productSelectionChanged(0);
    }
    void FactoryProductionTab::productSelectionChanged(unsigned int selection)
    {
        if (productList.size() == 0)
            return;
        auto product = productList[selection];
        productNameText->setText(product->getLocalisedName());
        productImage->loadImage(utils::os::combine("images", "products", product->getImage()));
        costsText->setTextF("%'.2f €/m", product->calculateCostsPerMonth());
        costsPerPieceText->setTextF("%'.2f €", product->calculateCostsPerPiece());

        productionCycleText->setTextF("%'d  - %'d", product->getProductionCycle().startMonth, product->getProductionCycle().endMonth);

        for (auto p : resourceComponents)
        {
            removeObject(p);
        }
        resourceComponents.clear();
        int y = 70;
        for (auto product : product->getBaseProducts())
        {
            std::shared_ptr<UI::ProductComponent> pc = std::make_shared<UI::ProductComponent>(product->product, this);

            resourceComponents.push_back(pc);
            pc->setPos(180, y);
            addObject(pc);
            y += 55;
        }

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
