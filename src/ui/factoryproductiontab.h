#ifndef FACTORYPRODUCTIONTAB_H
#define FACTORYPRODUCTIONTAB_H

#include <engine/ui/Tab.h>
#include <engine/ui/Button.h>
#include <engine/ui/Label.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/ComboBox.h>
#include "services/productservice.h"
#include "world/building.h"
#include "ui/productcomponent.h"
#include "world/company.h"

namespace UI
{

    class FactoryProductionTab : public UI::Tab
    {
    public:
        FactoryProductionTab(UI::Object *parent, std::shared_ptr<world::Building> building, const std::shared_ptr<world::Company> &player);

        void setBuilding(std::shared_ptr<world::Building> building);

    private:
        void initUI();
        void resourceSelectionChanged(unsigned int selection);
        void productSelectionChanged(unsigned int selection);
        void refreshProductList();
        std::shared_ptr<world::Building> building;
        std::shared_ptr<world::Company> player;
        std::vector<std::shared_ptr<Product>> productList;
        std::vector<std::shared_ptr<Product>> baseProductList;

        std::shared_ptr<UI::Label> descriptionText;
        std::shared_ptr<UI::Label> productionCycleText;
        std::shared_ptr<UI::Label> costsText;
        std::shared_ptr<UI::Label> costsPerPieceText;
        std::shared_ptr<UI::Label> productNameText;
        std::shared_ptr<UI::ComboBox<std::shared_ptr<Product>>> productSelectionBox;
        std::shared_ptr<UI::ComboBox<std::shared_ptr<Product>>> resourceSelectionBox;
        std::shared_ptr<UI::ImageButton> productImage;
        std::shared_ptr<UI::Button> addButton;
        std::shared_ptr<UI::Button> helpButton;
        std::vector<std::shared_ptr<UI::ProductComponent>> resourceComponents;
        std::vector<std::shared_ptr<UI::ProductComponent>> productComponents;
    };
}

#endif // FACTORYPRODUCTIONTAB_H
