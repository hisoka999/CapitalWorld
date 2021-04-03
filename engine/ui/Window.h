#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "engine/core/input.h"
#include "engine/core/renderer.h"
#include "engine/ui/Button.h"
#include "engine/ui/Container.h"
#include "engine/ui/Object.h"

namespace UI
{

    class Window : public Container, public Object
    {
    public:
        /** Default constructor */
        Window();
        Window(int x, int y, int width, int height);
        /** Default destructor */
        virtual ~Window();

        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);
        void setVisible(bool visible)
        {
            this->visible = visible;
            if (!visible)
            {
                this->fireFuncionCall("closed");
            }
            else
            {
                onOpen();
            }
        }
        bool getVisible()
        {
            return visible;
        }

        void setTitle(std::string title)
        {
            this->title = title;
        }

        virtual graphics::Rect displayRect();
        virtual graphics::Rect eventRect();

        void setSize(int width, int height)
        {
            this->width = width;
            this->height = height;
        }
        virtual void clear();

    protected:
        virtual void onOpen(){

        };

    private:
        int width, height;
        bool visible;
        virtual void buttonClick();
        std::shared_ptr<UI::Button> closeButton;
        std::string title;
        static int windowCount;
    };

} // namespace UI

#endif // UI_WINDOW_H
