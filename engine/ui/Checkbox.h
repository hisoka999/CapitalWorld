#ifndef UI_CHECKBOX_H
#define UI_CHECKBOX_H

#include <engine/ui/Object.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/text.h>
#include <iostream>

namespace UI {

class Checkbox : public UI::Object
{
    public:
        /** Default constructor */
        Checkbox(UI::Object *parent);
        /** Default destructor */
        virtual ~Checkbox();
        bool isChecked(){return checked;}
        void setChecked(bool check){checked = check;}
        void toggleChecked(){checked= !checked;}
        void setText(std::string text){this->text = text;}
        virtual void render(core::Renderer *pRender,graphics::Texture *pTexture);
        virtual void handleEvents(core::Input *pInput);
    protected:
    private:
        bool checked;
        std::string text;
        graphics::Text *textCore;
        SDL_Color color;
};

} // namespace UI

#endif // UI_CHECKBOX_H
