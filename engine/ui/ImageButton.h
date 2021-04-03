#ifndef UI_IMAGEBUTTON_H
#define UI_IMAGEBUTTON_H

#include <engine/graphics/texture.h>
#include <engine/ui/Object.h>
#include <memory>

namespace UI
{

    enum class ButtonState
    {
        None,
        Clicked,
        Hovered
    };

    class ImageButton : public UI::Object
    {
    public:
        /** Default constructor */
        ImageButton(Object *parent, int width, int height, int x2, int y2,
                    bool pResized = false);
        /** Default destructor */
        virtual ~ImageButton();
        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);
        void loadImage(std::string pFilename);
        void loadClickedImage(std::string pFilename);
        void setImage(std::shared_ptr<graphics::Texture> pImage)
        {
            image = pImage;
        }
        bool imageLoaded()
        {
            return image != nullptr;
        }
        void setText(std::string text);
        void setDefaultColor(SDL_Color color);
        void setHoverColor(SDL_Color color);
        void setClickColor(SDL_Color color);

    protected:
        std::shared_ptr<graphics::Texture> image;
        std::shared_ptr<graphics::Texture> clickedImage;

    private:
        SDL_Color defaultColor;
        SDL_Color hoverColor;
        SDL_Color clickColor;
        std::string text;
        int x2;
        int y2;
        bool resized;
        ButtonState state;
    };

} // namespace UI

#endif // UI_IMAGEBUTTON_H
