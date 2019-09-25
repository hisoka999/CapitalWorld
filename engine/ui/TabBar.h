/*
 * TabBar.h
 *
 *  Created on: 03.06.2016
 *      Author: stefan
 */

#ifndef UI_TABBAR_H_
#define UI_TABBAR_H_

#include "engine/ui/Object.h"
#include "engine/ui/Tab.h"
#include <vector>
#include <memory>

namespace UI
{

class TabBar: public UI::Object
{
public:
    TabBar(Object* parent = nullptr);
    virtual ~TabBar();

    void addTab(std::shared_ptr<Tab> tab);
    void removeTabByIndex(int index);
    void removeTab(std::shared_ptr<Tab> tab);

    virtual void render(core::Renderer *pRender, graphics::Texture *texture);
    virtual void handleEvents(core::Input *pInput);
    virtual graphics::Rect displayRect();
private:
    std::vector<std::shared_ptr<Tab>> tabs;
    int currentTab;
};

} /* namespace UI */

#endif /* UI_TABBAR_H_ */
