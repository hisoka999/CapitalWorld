#ifndef __LINE_H__
#define __LINE_H__

#include <engine/ui/Object.h>

namespace UI
{

    class Line : public UI::Object
    {

    public:
        Line(UI::Object *parent, const utils::Vector2 &start, const utils::Vector2 &end);
        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);

    private:
        utils::Vector2 startPos;
        utils::Vector2 endPos;
    };
}; // namespace UI

#endif // __LINE_H__