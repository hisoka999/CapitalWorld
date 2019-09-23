#ifndef UI_SCROLLAREA_H
#define UI_SCROLLAREA_H

#include <engine/ui/Container.h>
#include <engine/ui/Object.h>

namespace UI
{

class ScrollArea: public UI::Container, public UI::Object
{
public:
    /** Default constructor */
    ScrollArea(const unsigned int pWidth, const unsigned pHeight,
            UI::Object* parent);
    /** Default destructor */
    virtual ~ScrollArea();
    virtual void render(core::Renderer *pRender, graphics::Texture *pTexture);
    virtual graphics::Rect displayRect();
    virtual void handleEvents(core::Input *pInput);
    void reset();
protected:
private:
    graphics::Rect renderRect;
    int scrollWidth, scrollHeight;
    int scrollX, scrollY;
    int scrollPosX, scrollPosY;
    graphics::Texture *renderArea;
    bool buttonPressed;
};

} // namespace UI

#endif // UI_SCROLLAREA_H
