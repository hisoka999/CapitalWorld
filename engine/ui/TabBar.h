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
#include <memory>
#include <vector>

namespace UI
{

    class TabBar : public UI::Object
    {
    public:
        TabBar(Object *parent = nullptr);
        virtual ~TabBar();

        void addTab(std::shared_ptr<Tab> tab);
        void removeTabByIndex(int index);
        void removeTab(std::shared_ptr<Tab> tab);

        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);
        virtual graphics::Rect displayRect();
        virtual graphics::Rect eventRect();
        void setCurrentTab(std::shared_ptr<Tab> tab);

    protected:
        const int tabWidth = 120;
        const int tabHeight = 30;

    private:
        std::vector<std::shared_ptr<Tab>> tabs;
        int currentTab;
        int hoveredTab;
        std::shared_ptr<graphics::Texture> btnTexture;
    };

} /* namespace UI */

#endif /* UI_TABBAR_H_ */
