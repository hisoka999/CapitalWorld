#include "BuildingSelectionComponent.h"
#include "world/company.h"
#include <engine/graphics/TextureManager.h>
#include <engine/ui/ImageButton.h>
#include <engine/ui/Label.h>
#include <engine/utils/color.h>
#include <engine/utils/os.h>
#include "translate.h"
namespace UI
{
    BuildingSelectionComponent::BuildingSelectionComponent(UI::Object *parent, std::shared_ptr<world::Building> &building, const std::shared_ptr<world::Company> &company)
        : UI::Container(), UI::Object(parent), building(building), company(company)
    {
        setObjectName("BuildingSelectionComponent");
        initUI();
        setWidth(270);
        setHeight(110);
        if (getTheme() == nullptr)
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());

        m_backgroundColor = getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor);
        m_selectedColor = getTheme()->getStyleColor(this, UI::StyleType::HoverColor);
    }

    BuildingSelectionComponent::~BuildingSelectionComponent()
    {
    }

    void BuildingSelectionComponent::render(core::Renderer *pRender)
    {
        if (selected)
            pRender->setDrawColor(m_selectedColor);
        else
            pRender->setDrawColor(m_backgroundColor);

        pRender->fillRect(displayRect());

        UI::Object::render(pRender);
        UI::Container::render(pRender);
    }

    bool BuildingSelectionComponent::handleEvents(core::Input *pInput)
    {
        bool eventHandled = UI::Container::handleEvents(pInput);
        selected = eventRect().intersects(pInput->getMousePostion());
        if (selected && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            fireFuncionCall("clicked", building);
            eventHandled = true;
        }

        return eventHandled;
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
        costsLabel->setTextF(_("Price: %s"), format_currency(building->getBuildPrice()));
        costsLabel->setPos(140, 30);
        if (building->canBuild(company->getCash()))
        {
            costsLabel->setColor(utils::color::GREEN);
        }
        else
        {
            costsLabel->setColor(utils::color::RED);
        }
        addObject(costsLabel);
    }
}