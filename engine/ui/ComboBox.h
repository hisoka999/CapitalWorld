#ifndef UI_COMBOBOX_H
#define UI_COMBOBOX_H

#include <engine/ui/Object.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/text.h>
#include <vector>

namespace UI {

class ComboBox : public UI::Object
{
    public:
        /** Default constructor */
        ComboBox(UI::Object *parent = 0);
        /** Default destructor */
        virtual ~ComboBox();
        virtual void render(core::Renderer *pRender,graphics::Texture *pTexture);
        void setWidth(int width){this->width = width;}
        int getWidth(){return width;}
        void addElement(std::string elem){elements.push_back(elem);}
        void clearElements(){elements.clear(); selection = 0;}
        virtual void handleEvents(core::Input *pInput);
        int getSelection() { return selection;}
        void setSelection(unsigned int selection){this->selection = selection;}
    protected:
    private:
        int width;
        std::vector<std::string> elements;
        bool mouseDown;
        unsigned int selection;
};

} // namespace UI

#endif // UI_COMBOBOX_H
