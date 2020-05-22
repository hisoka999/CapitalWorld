/*
 * TabBar.cpp
 *
 *  Created on: 03.06.2016
 *      Author: stefan
 */

#include "engine/ui/TabBar.h"

namespace UI
{

TabBar::TabBar(Object* parent) :
        UI::Object(parent), currentTab(-1)
{

}

TabBar::~TabBar()
{
    // TODO Auto-generated destructor stub
}
void TabBar::render(core::Renderer *pRender, graphics::Texture *texture)
{
    //render tabbar
    graphics::Rect displayRect;
    if (getParent() != nullptr)
    {
        displayRect = getParent()->displayRect();
    }
    int index = 0;
    SDL_Color color =
    { 255, 255, 255, 255 };
    int tabX = displayRect.x + 5;
    int taby = displayRect.y + 5;
    graphics::Rect tabRect;
    graphics::Rect tabBarRect;
    tabBarRect.x = tabX;

    for (auto& tab : tabs)
    {
        std::string title = tab->getTitle();
        //render tab
        int w = 0;
        int h = 0;
        getFont()->size(title, &w, &h);

        //render background
        tabRect.x = tabX;
        tabRect.y = taby;
        tabRect.width = w + 20;
        tabRect.height = h + 6;
        tabBarRect.y = tabRect.height+taby;
        tabBarRect.height = getHeight() - tabRect.height;
        //44d3d5
        if (index == currentTab)
            pRender->setDrawColor(0x44, 0xd3, 0xd5, 255);
        else
            pRender->setDrawColor(0x3c, 0xb5, 0xb5, 255);
        pRender->fillRect(tabRect);
        pRender->setDrawColor(255, 255, 255, 255);
        pRender->drawRect(tabRect);
        tabX += 10; // offset
        //render title
        getFont()->render(pRender, title, color, tabX, taby + 3);
        tabX += w + 10;
        index++;

    }
    tabBarRect.width = getWidth() - 5;
    pRender->setDrawColor(255, 255, 255, 255);
    pRender->drawRect(tabBarRect);

    if (currentTab != -1)
    {
        tabs[currentTab]->render(pRender, texture);
    }
}
void TabBar::handleEvents(core::Input *pInput)
{
    if (currentTab != -1)
    {
        tabs[currentTab]->handleEvents(pInput);
        //handle tabbar events

        graphics::Rect displayRect;
        if (getParent() != nullptr)
        {
            displayRect = getParent()->displayRect();
        }
        int tabX = displayRect.x + 5;
        int taby = displayRect.y + 5;
        graphics::Rect tabRect;
        int index = 0;
        for (auto& tab : tabs)
        {
            std::string title = tab->getTitle();
            //render tab
            int w = 0;
            int h = 0;
            getFont()->size(title, &w, &h);

            //render background
            tabRect.x = tabX;
            tabRect.y = taby;
            tabRect.width = w + 20;
            tabRect.height = h + 6;

            if (tabRect.intersects(pInput->getMousePostion()))
            {
                if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
                {
                    currentTab = index;
                }
                //hover
            }

            tabX += 10; // offset
            tabX += w + 10;
            index++;
        }
    }

}

void TabBar::addTab(std::shared_ptr<Tab> tab)
{
    tabs.push_back(tab);
    if (currentTab == -1)
        currentTab = 0;
}
void TabBar::removeTabByIndex(int index)
{
    auto it = tabs.begin() + index;
    tabs.erase(it);
}
void TabBar::removeTab(std::shared_ptr<Tab> tab)
{
    auto it = std::find(tabs.begin(), tabs.end(), tab);
    if (it != tabs.end()){
        tabs.erase(it);
        currentTab = 0;
    }
}

graphics::Rect TabBar::displayRect()
{
    graphics::Rect r;
    r.x = getX() + 5;
    r.y = getY() + 30;
    if (getParent() != nullptr)
    {
        r.x += getParent()->displayRect().x;
        r.y += getParent()->displayRect().y;
    }
    return r;
}

void TabBar::setCurrentTab(std::shared_ptr<Tab> tab)
{
    auto it = std::find(tabs.begin(), tabs.end(), tab);
    currentTab = std::distance( tabs.begin(),it);
}

} /* namespace UI */
