#include "BuildingSelectionComponent.h"
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>

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
        auto textureMap = graphics::TextureManager::Instance().loadTextureMap(utils::os::combine("images", "tiles", "iso_tiles.json"));

        buildingImage->setPos(5, 5);
        buildingImage->setImage(textureMap->getTexture());
        addObject(buildingImage);
        auto nameLabel = std::make_shared<UI::Label>(building->getDisplayName(), this);
        nameLabel->setFont("fonts/arial.ttf", 14);
        nameLabel->setPos(150, 10);
        addObject(nameLabel);

        auto costsLabel = std::make_shared<UI::Label>(this);
        costsLabel->setFont("fonts/arial.ttf", 12);
        costsLabel->setTextF("Price: %d €", building->getBuildPrice());
        costsLabel->setPos(150, 30);
        addObject(costsLabel);
    }
}