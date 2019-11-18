#ifndef FARMPRODUCTIONTAB_H
#define FARMPRODUCTIONTAB_H

#include <engine/ui/Tab.h>
#include <engine/ui/Button.h>
#include <engine/ui/Label.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/ComboBox.h>
#include <services/productservice.h>
#include <world/building.h>
#include <ui/productcomponent.h>

namespace UI {

class FarmProductionTab : public UI::Tab
{
public:
    FarmProductionTab(UI::Object* parent = nullptr);

    void setBuilding(std::shared_ptr<world::Building> building);
private:
    void initUI();
    void resourceSelectionChanged(unsigned int selection);
    void productSelectionChanged(unsigned int selection);
    void refreshProductList();
    std::shared_ptr<world::Building> building;
    std::vector<std::shared_ptr<Product>> productList;
    std::vector<std::shared_ptr<Resource>> resourceList;

    std::shared_ptr<UI::Label> descriptionText;
    std::shared_ptr<UI::Label> productionCycleText;
    std::shared_ptr<UI::Label> landTypeText;
    std::shared_ptr<UI::Label> costsText;
    std::shared_ptr<UI::Label> productNameText;
    std::shared_ptr<UI::Label> resourceNameText;
    std::shared_ptr<UI::ComboBox> productSelectionBox;
    std::shared_ptr<UI::ComboBox> resourceSelectionBox;
    std::shared_ptr<UI::ImageButton> productImage;
    std::shared_ptr<UI::ImageButton> resourceImage;
    std::shared_ptr<UI::Button> addButton;
    std::shared_ptr<UI::Button> helpButton;
    std::vector<std::shared_ptr<UI::ProductComponent>> productComponents;

};
}

#endif // FARMPRODUCTIONTAB_H
