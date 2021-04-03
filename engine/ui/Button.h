#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <engine/graphics/rect.h>
#include <engine/graphics/text.h>
#include <engine/graphics/texture.h>
#include <engine/ui/Object.h>
#include <iostream>
#include <string>

namespace UI
{

    class Button : public UI::Object
    {
    public:
        /** Default constructor */
        Button(Object *parent = nullptr);
        /** Default destructor */
        virtual ~Button();

        void setLabel(const std::string &label);
        void setFont(const std::string &fontname, unsigned int font_size);
        void setStaticWidth(const int pWidth);
        void setColor(SDL_Color color);
        void enable();
        void disable();
        bool isEnabled();
        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);
        virtual graphics::Rect displayRect();
        virtual graphics::Rect eventRect();

        static std::string buttonClickCallback()
        {
            return "buttonClick";
        }

        bool getBorderless() const;
        void setBorderless(bool value);

        bool isHovered() const;

        SDL_Color getHoverColor() const;
        void setHoverColor(const SDL_Color &value);

        SDL_Color getDisabledColor() const;
        void setDisabledColor(const SDL_Color &value);

    protected:
        void renderBackground(core::Renderer *pRenderer);
        std::string label;
        SDL_Color color;

    private:
        int width;
        int static_width;
        bool hover;
        bool enabled;
        bool borderless;
        std::shared_ptr<graphics::Texture> texture;
        SDL_Color hoverColor;
        SDL_Color disabledColor;
    };

} // namespace UI

#endif // UI_BUTTON_H
