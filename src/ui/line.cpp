#include "line.h"

namespace UI
{
    Line::Line(UI::Object *parent, const utils::Vector2 &start, const utils::Vector2 &end) : UI::Object(parent), startPos(start), endPos(end)
    {
    }

    void Line::render(core::Renderer *pRender)
    {
        pRender->setDrawColor(0, 200, 200, 255);
        pRender->drawLine(startPos, endPos);
    }

    bool Line::handleEvents([[maybe_unused]] core::Input *pInput)
    {
        return false;
    }
} // namespace UI