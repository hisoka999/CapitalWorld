#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "engine/ui/Container.h"
#include "engine/ui/Object.h"
#include "engine/ui/ImageButton.h"
#include "engine/ui/ButtonListener.h"
#include "engine/core/input.h"
#include "engine/core/renderer.h"

namespace UI {

class Window: public Container, public Object, private UI::ButtonListener {
public:
    /** Default constructor */
    Window();
    Window(int x, int y, int width, int height);
    /** Default destructor */
    virtual ~Window();

    virtual void render(core::Renderer *pRender, graphics::Texture *pTexture);
    virtual void handleEvents(core::Input *pInput);
    void setVisible(bool visible) {
        this->visible = visible;
        if (!visible) {
            this->fireFuncionCall("closed");
        } else {
            onOpen();
        }

    }
    bool getVisible() {
        return visible;
    }

    void setTitle(std::string title) {
        this->title = title;
    }

    virtual graphics::Rect displayRect();

    void setSize(int width, int height) {
        this->width = width;
        this->height = height;
    }
    virtual void clear();
protected:
    virtual void onOpen() {

    }
    ;
private:
    int width, height;
    bool visible;
    virtual void buttonClick();
    UI::ImageButton *closeButton;
    std::string title;
    static int windowCount;

};

} // namespace UI

#endif // UI_WINDOW_H
