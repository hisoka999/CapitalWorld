#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <engine/ui/Object.h>
#include <string>
#include <iostream>
#include <engine/ui/ButtonListener.h>
#include <engine/graphics/rect.h>
#include <engine/graphics/text.h>
#include <engine/graphics/texture.h>

namespace UI {

class Button: public UI::Object {
public:
    /** Default constructor */
    Button(UI::Object *parent = 0);
    /** Default destructor */
    virtual ~Button();

    void setLabel(const std::string& label);
    void setFont(const std::string& fontname, unsigned int font_size);
    void setStaticWidth(const int pWidth);
    void setColor(SDL_Color color);
    void enable();
    void disable();
    bool isEnabled();
    virtual void render(core::Renderer *pRender, graphics::Texture *texture);
    virtual void handleEvents(core::Input *pInput);
    //static constexpr std::string CALL_BUTTON_CLICK = "buttonClick";
    static std::string buttonClickCallback() {
        return "buttonClick";
    }
protected:
private:
    std::string label;
    SDL_Color color;
    int width;
    int static_width;
    bool hover;
    bool enabled;
};

} // namespace UI

#endif // UI_BUTTON_H
