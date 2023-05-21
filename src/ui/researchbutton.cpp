#include "researchbutton.h"
#include "engine/graphics/TextureManager.h"
#include "translate.h"
#include <engine/utils/os.h>
#include <engine/utils/string.h>
namespace UI
{
    ResearchButton::ResearchButton(const std::shared_ptr<Research> &research, const std::shared_ptr<world::GameState> &gameState, UI::Object *parent)
        : UI::Object(parent), research(research), gameState(gameState)
    {
        setObjectName("ResearchButton");
        iconFont = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);
        textFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "Audiowide-Regular.ttf"), 12);

        smallIconFont = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 8);
        smallTextFont = graphics::TextureManager::Instance().loadFont(utils::os::combine("fonts", "Audiowide-Regular.ttf"), 8);

        setWidth(250);
        setHeight(40);
        if (parent == nullptr || getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }
        backgroundColor = getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor);
        // = getTheme()->getStyleColor(this,UI::StyleType::BackgroundColor);
    }

    ResearchButton::~ResearchButton()
    {
        iconFont = nullptr;
        textFont = nullptr;
        smallIconFont = nullptr;
        smallTextFont = nullptr;
    }

    void ResearchButton::render(core::Renderer *pRender)
    {
        auto rect = displayRect();
        pRender->setDrawColor(backgroundColor);

        pRender->fillRect(rect);
        if (research->canResearch(gameState->getTime()))
            pRender->setDrawColor(0, 200, 200, 255);
        else
        {
            pRender->setDrawColor(93, 103, 108, 255);
        }
        pRender->drawRect(rect);

        auto iconRect = rect;

        iconRect.width = rect.height;
        pRender->fillRect(iconRect);
        SDL_Color textColor = {255, 255, 255, 0};
        if (hovered)
        {
            textColor = {93, 103, 108, 255};
        }
        iconFont->render(pRender, research->getIcon(), textColor, rect.x + 10, rect.y + 10);

        textFont->render(pRender, research->getLocalisedName(), textColor, rect.x + rect.height + 10, rect.y + 5);

        // draw research time
        if (research->getResearched())
        {
            smallTextFont->render(pRender, _("researched"), textColor, rect.x + rect.width - 70, rect.y + rect.height - 10);
        }
        else
        {
            smallIconFont->render(pRender, "\uf254", textColor, rect.x + rect.width - 70, rect.y + rect.height - 10);
            int resPerMonth = (gameState->getPlayer()->getResearchPerMonth() == 0) ? 1 : gameState->getPlayer()->getResearchPerMonth();
            auto researchTime = utils::string_format("%i months", research->getCurrentCosts() / resPerMonth);
            smallTextFont->render(pRender, researchTime, textColor, rect.x + rect.width - 60, rect.y + rect.height - 10);
        }
    }

    bool ResearchButton::handleEvents(core::Input *pInput)
    {
        bool eventHandled = UI::Object::handleEvents(pInput);
        auto rect = eventRect();
        if (rect.intersects(pInput->getMousePostion()) && research->canResearch(gameState->getTime()) && !research->getResearched())
        {
            hovered = true;
            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                this->fireFuncionCall("buttonClicked");
                eventHandled = true;
            }
        }
        else
        {
            hovered = false;
        }
        return eventHandled;
    }

    const std::shared_ptr<Research> &ResearchButton::getResearch()
    {
        return research;
    }

}
