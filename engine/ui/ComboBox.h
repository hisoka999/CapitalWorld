#ifndef UI_COMBOBOX_H
#define UI_COMBOBOX_H

#include <engine/graphics/text.h>
#include <engine/graphics/texture.h>
#include <engine/ui/Object.h>
#include <vector>
#include <type_traits>
#include <engine/graphics/TextureManager.h>

namespace UI
{

    template <typename T>
    class ComboBox : public UI::Object
    {
    public:
        /** Default constructor */
        ComboBox(Object *parent = nullptr) : UI::Object(parent), selection(0)
        {
            width = 100;
            iconFont = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);

            mouseDown = false;
            renderOrder = 99;

            elementFunction = nullptr;
        }
        /** Default destructor */
        virtual ~ComboBox()
        {
        }

        void render(core::Renderer *pRender)
        {

            int tx = getX();

            int ty = getY();

            if (this->getParent())
            {
                graphics::Rect dsp = getParent()->displayRect();
                tx += dsp.x;
                ty += dsp.y;
            }

            graphics::Rect rect;
            rect.x = tx;
            rect.y = ty;
            rect.width = getWidth();
            rect.height = 28;

            SDL_Color textColor = {255, 255, 255, 255};
            SDL_Color selectionColor = {93, 103, 108, 255};

            graphics::Rect leftButtonRect = {rect.x, rect.y, rect.height, rect.height};
            graphics::Rect rightButtonRect = {rect.x + rect.width - rect.height, rect.y, rect.height, rect.height};
            pRender->setDrawColor(12, 21, 24, 255);

            pRender->fillRect(rect);
            pRender->setDrawColor(93, 103, 108, 255);
            pRender->fillRect(leftButtonRect);
            pRender->fillRect(rightButtonRect);

            // draw left and right button filling
            if (selection > 0)
                pRender->setDrawColor(0, 200, 200, 255);
            else
                pRender->setDrawColor(93, 103, 108, 255);
            pRender->fillRect(leftButtonRect);
            if (selection < elements.size() - 1)
                pRender->setDrawColor(0, 200, 200, 255);
            else
                pRender->setDrawColor(93, 103, 108, 255);

            pRender->fillRect(rightButtonRect);

            iconFont->render(pRender, "\uf0da", textColor, rightButtonRect.x + (rect.height / 2) - 5, rightButtonRect.y + (rect.height / 2) - 10);
            iconFont->render(pRender, "\uf0d9", textColor, leftButtonRect.x + (rect.height / 2) - 5, leftButtonRect.y + (rect.height / 2) - 10);

            std::string text = elementFunction(elements[selection]);

            int textW, textH = 0;
            getFont()->size(text, &textW, &textH);

            getFont()->render(pRender, text, textColor, rect.x + rect.height + 5, rect.y + (rect.height / 2) - (textH / 2));
        }
        void setWidth(int width) { this->width = width; }
        int getWidth() { return width; }
        void addElement(T elem) { elements.push_back(elem); }
        void setElementFunction(std::function<std::string(T)> func)
        {
            elementFunction = func;
        }
        void clearElements()
        {
            elements.clear();
            selection = 0;
        }
        virtual void handleEvents(core::Input *pInput)
        {
            SDL_Event e = pInput->getEvent();
            int tx = getX();

            int ty = getY();

            if (this->getParent())
            {
                graphics::Rect dsp = getParent()->displayRect();
                tx += dsp.x;
                ty += dsp.y;
            }

            graphics::Rect rect;
            rect.x = tx;
            rect.y = ty;
            rect.width = getWidth();
            rect.height = 28;

            graphics::Rect leftButtonRect = {rect.x, rect.y, rect.height, rect.height};
            graphics::Rect rightButtonRect = {rect.x + rect.width - rect.height, rect.y, rect.height, rect.height};

            if (leftButtonRect.intersects(pInput->getMousePostion()) && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                if (selection > 0)
                {
                    setSelection(selection - 1);
                }
            }
            else if (rightButtonRect.intersects(pInput->getMousePostion()) && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                if (selection < elements.size() - 1)
                {
                    setSelection(selection + 1);
                }
            }
        }
        int getSelection() { return selection; }
        void setSelection(unsigned int selection)
        {
            this->selection = selection;
            this->fireFuncionCall("selectionChanged", selection);
            if (selection >= 0)
                fireFuncionCall("valueChanged", elements[selection]);
        }

        void setSelectionByText(T text)
        {
            // Get index of element from iterator
            int index = -1;
            auto it = std::find(elements.begin(), elements.end(), text);
            if (it != elements.end())
            {
                index = std::distance(elements.begin(), it);
                setSelection(index);
            }
        }

    protected:
    private:
        int width;
        std::vector<T> elements;
        bool mouseDown;
        unsigned int selection;
        std::shared_ptr<graphics::Text> iconFont;

        std::function<std::string(T)> elementFunction;
    };

} // namespace UI

#endif // UI_COMBOBOX_H
