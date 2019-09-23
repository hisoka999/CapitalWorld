#ifndef UI_BUTTONLISTENER_H
#define UI_BUTTONLISTENER_H

#include <engine/ui/Object.h>

namespace UI {

class ButtonListener : public UI::Listener
{
    public:
        /** Default constructor */
        ButtonListener();
        /** Default destructor */
        virtual ~ButtonListener();
        virtual void buttonClick();
        void setParent(UI::Object *parent){this->parent = parent;}
        UI::Object *getParent() {return parent;}
    protected:
    private:
        UI::Object* parent;
};

} // namespace UI

#endif // UI_BUTTONLISTENER_H
