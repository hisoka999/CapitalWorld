#ifndef RESOURCEPRODUCTIONTAB_H
#define RESOURCEPRODUCTIONTAB_H

#include "../services/productservice.h"
#include "../ui/productcomponent.h"
#include "../world/building.h"
#include <engine/ui/Button.h>
#include <engine/ui/ComboBox.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>
#include <engine/ui/Tab.h>

namespace UI
{

    class ResourceProductionTab : public UI::Tab
    {
    public:
        ResourceProductionTab(UI::Object *parent, std::shared_ptr<world::Building> building, world::RawResource rawResource);

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
        std::shared_ptr<UI::ComboBox<std::string>> productSelectionBox;
        std::shared_ptr<UI::ComboBox<std::string>> resourceSelectionBox;
        std::shared_ptr<UI::ImageButton> productImage;
        std::shared_ptr<UI::ImageButton> resourceImage;
        std::shared_ptr<UI::Button> addButton;
        std::shared_ptr<UI::Button> helpButton;
        std::vector<std::shared_ptr<UI::ProductComponent>> productComponents;
        world::RawResource rawResource;
    };
}

#endif // RESOURCEPRODUCTIONTAB_H
