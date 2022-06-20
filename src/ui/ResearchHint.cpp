#include "ResearchHint.h"
#include "translate.h"
#include <engine/graphics/TextureManager.h>
#include <engine/utils/color.h>
#include <engine/utils/string.h>
#include <iostream>

namespace UI
{
    ResearchHint::ResearchHint(const std::shared_ptr<Research> &research)
        : UI::Hint(), m_research(research)
    {
        setWidth(250);
        setHeight(180);

        titleFont = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 14);
        contentFont = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);
    }

    ResearchHint::~ResearchHint()
    {
    }

    void ResearchHint::render(core::Renderer *renderer)
    {
        renderBackground(renderer);

        const utils::Vector2 &pos = getPosition();
        if (pos.getX() <= 5.f)
        {
            std::cout << "position is wrong" << std::endl;
        }

        titleFont->setStyle(graphics::FontStyle::BOLD);
        titleFont->render(renderer, m_research->getLocalisedName(), utils::color::WHITE, pos.getX() + 10, pos.getY());
        contentFont->renderWrapped(renderer, m_research->getDescription(), utils::color::WHITE, pos.getX() + 10, pos.getY() + 20, getWidth() - 20);

        // TODO add price
        std::string costs = utils::string_format(_("Researchtime: %d months"), m_research->getCosts());
        std::string researchDate = utils::string_format(_("Available at: %s"), m_research->getResearchDate().format());

        contentFont->render(renderer, costs, utils::color::RED, pos.getX() + 10, pos.getY() + 80);
        contentFont->render(renderer, researchDate, utils::color::WHITE, pos.getX() + 10, pos.getY() + 95);
        contentFont->setStyle(graphics::FontStyle::BOLD);
        contentFont->render(renderer, _("Requirements:"), utils::color::WHITE, pos.getX() + 10, pos.getY() + 110);
        contentFont->setStyle(graphics::FontStyle::NORMAL);
        float yOffset = pos.getY() + 125;
        for (auto &enables : m_research->getRequirements())
        {
            contentFont->render(renderer, " - " + enables->getLocalisedName(), utils::color::WHITE, pos.getX() + 10, yOffset);

            yOffset += 15;
        }
    }
}