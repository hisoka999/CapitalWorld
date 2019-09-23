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

class Tab: public UI::Container, public UI::Object
{
public:
    Tab(UI::Object* parent, std::string title);
    virtual ~Tab();
    std::string getTitle();
    void setTitle(std::string title);
    virtual void render(core::Renderer *pRender, graphics::Texture *pTexture);
    virtual void handleEvents(core::Input *pInput);
private:
    std::string title;
};

} /* namespace UI */

#endif /* UI_TAB_H_ */
