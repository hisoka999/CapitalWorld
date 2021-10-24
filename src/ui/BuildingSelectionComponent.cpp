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
        setWidth(270);
        setHeight(110);
    }

    BuildingSelectionComponent::~BuildingSelectionComponent()
    {
    }

    void BuildingSelectionComponent::render(core::Renderer *pRender)
    {
        if (selected)
            pRender->setDrawColor(0x1e, 0xb9, 0xe5, 0xFF);
        else
            pRender->setDrawColor(0x0e, 0xa9, 0xe5, 0xFF);

        pRender->fillRect(displayRect());

        UI::Object::render(pRender);
        UI::Container::render(pRender);
    }

    void BuildingSelectionComponent::handleEvents(core::Input *pInput)
    {
        UI::Container::handleEvents(pInput);
        selected = eventRect().intersects(pInput->getMousePostion());
    }

    void BuildingSelectionComponent::initUI()
    {
        auto srcRect = building->getSourceRect();

        std::shared_ptr<UI::ImageButton> buildingImage = std::make_shared<UI::ImageButton>(this, srcRect.width, srcRect.height, srcRect.x, srcRect.y);
        setHeight(srcRect.height + 5);
        auto textureMap = graphics::TextureManager::Instance().loadTextureMap(utils::os::combine("images", "tiles", "iso_tiles.json"));

        buildingImage->setPos(5, 5);
        buildingImage->setImage(textureMap->getTexture());
        addObject(buildingImage);
        auto nameLabel = std::make_shared<UI::Label>(building->getDisplayName(), this);
        nameLabel->setFont("fonts/arial.ttf", 14);
        nameLabel->getFont()->setStyle(graphics::FontStyle::BOLD);
        nameLabel->setPos(140, 10);
        addObject(nameLabel);

        auto costsLabel = std::make_shared<UI::Label>(this);
        costsLabel->setFont("fonts/arial.ttf", 12);
        costsLabel->setTextF("Price: %d €", building->getBuildPrice());
        costsLabel->setPos(140, 30);
        addObject(costsLabel);
    }
}