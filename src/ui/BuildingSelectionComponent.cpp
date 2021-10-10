#include "BuildingSelectionComponent.h"
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>

namespace UI
{
    BuildingSelectionComponent::BuildingSelectionComponent(UI::Object *parent, std::shared_ptr<world::Building> &building)
        : UI::Container(), UI::Object(parent), building(building)
    {
        initUI();
    }

    BuildingSelectionComponent::~BuildingSelectionComponent()
    {
    }

    void BuildingSelectionComponent::render(core::Renderer *pRender)
    {
        UI::Object::render(pRender);
        UI::Container::render(pRender);
    }

    void BuildingSelectionComponent::handleEvents(core::Input *pInput)
    {
        UI::Container::handleEvents(pInput);
    }

    void BuildingSelectionComponent::initUI()
    {
        auto srcRect = building->getSourceRect();
        std::shared_ptr<UI::ImageButton> buildingImage = std::make_shared<UI::ImageButton>(this, srcRect.width, srcRect.height, srcRect.x, srcRect.y);
        buildingImage->setPos(5, 5);
        addObject(buildingImage);
        auto nameLabel = std::make_shared<UI::Label>(building->getDisplayName(), this);
        nameLabel->setFont("fonts/arial.ttf", 14);
        nameLabel->setPos(110, 10);
        addObject(nameLabel);

        auto costsLabel = std::make_shared<UI::Label>(this);
        costsLabel->setFont("fonts/arial.ttf", 12);
        costsLabel->setTextF("Price: %.2f", building->getBuildPrice());
        costsLabel->setPos(110, 30);
        addObject(costsLabel);
    }
}