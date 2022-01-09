#pragma once

#include "world/research.h"
#include <engine/graphics/text.h>
#include <engine/ui/Hint.h>
namespace UI
{
    class ResearchHint : public UI::Hint
    {

    public:
        ResearchHint(const std::shared_ptr<Research> &research);
        ~ResearchHint();
        virtual void render(core::Renderer *renderer);

    private:
        std::shared_ptr<Research> m_research;
        std::shared_ptr<graphics::Text> titleFont;
        std::shared_ptr<graphics::Text> contentFont;
    };

}