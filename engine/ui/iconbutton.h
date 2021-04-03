#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include "Button.h"
#include <engine/graphics/text.h>

namespace UI {

class IconButton : public UI::Button
{
public:
    IconButton(Object* parent = nullptr);

    virtual void render(core::Renderer* pRender);
    void setIconText(const std::string &value);

private:
    std::string iconText;
    std::shared_ptr<graphics::Text> iconFont;
};

}

#endif // ICONBUTTON_H
