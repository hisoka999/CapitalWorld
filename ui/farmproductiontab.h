#ifndef FARMPRODUCTIONTAB_H
#define FARMPRODUCTIONTAB_H

#include <engine/ui/Tab.h>
#include <engine/ui/Button.h>
#include <engine/ui/Label.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/ComboBox.h>
#include <services/productservice.h>

namespace UI {

class FarmProductionTab : public UI::Tab
{
public:
    FarmProductionTab(UI::Object* parent = nullptr);

private:
    void initUI();
    std::shared_ptr<UI::Label> descriptionText;
    std::shared_ptr<UI::Label> productionCycleText;
    std::shared_ptr<UI::Label> landTypeText;
    std::shared_ptr<UI::Label> costsText;
    std::shared_ptr<UI::Label> productNameText;
    std::shared_ptr<UI::ComboBox> productSelectionBox;
    std::shared_ptr<UI::ImageButton> productImage;
    std::shared_ptr<UI::Button> addButton;


};
}

#endif // FARMPRODUCTIONTAB_H
