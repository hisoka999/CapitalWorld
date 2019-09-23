#ifndef UI_IMAGEBUTTON_H
#define UI_IMAGEBUTTON_H

#include <engine/graphics/texture.h>
#include <engine/ui/Object.h>
#include <engine/ui/ButtonListener.h>
#include <memory>


namespace UI {

class ImageButton: public UI::Object {
public:
    /** Default constructor */
    ImageButton(UI::Object *parent, int width, int height, int x2, int y2,
            bool pResized = false);
    /** Default destructor */
    virtual ~ImageButton();
    virtual void render(core::Renderer *pRender, graphics::Texture *texture);
    virtual void handleEvents(core::Input *pInput);
    void loadImage( std::string pFilename);
    void setImage(std::shared_ptr<graphics::Texture> pImage) {
        image = pImage;
    }
    bool imageLoaded() {
        return image != NULL;
    }
protected:
    std::shared_ptr<graphics::Texture> image;
private:
    int x2;
    int y2;
    bool resized;
};

} // namespace UI

#endif // UI_IMAGEBUTTON_H
