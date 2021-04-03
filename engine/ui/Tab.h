/*
 * Tab.h
 *
 *  Created on: 03.06.2016
 *      Author: stefan
 */

#ifndef UI_TAB_H_
#define UI_TAB_H_

#include "engine/ui/Container.h"
#include <string>

namespace UI
{

    class Tab : public UI::Container, public UI::Object
    {
    public:
        Tab(Object *parent, const std::string &title);
        virtual ~Tab();
        const std::string &getTitle() const;
        void setTitle(const std::string &title);
        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);

    private:
        std::string title;
    };

} /* namespace UI */

#endif /* UI_TAB_H_ */
