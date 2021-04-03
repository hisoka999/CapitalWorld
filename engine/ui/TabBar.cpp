/*
 * TabBar.cpp
 *
 *  Created on: 03.06.2016
 *      Author: stefan
 */

#include <engine/graphics/TextureManager.h>
#include <engine/ui/TabBar.h>

namespace UI
{

    TabBar::TabBar(Object *parent)
        : UI::Object(parent), currentTab(-1), hoveredTab(-1)
    {
        btnTexture = graphics::TextureManager::Instance().loadTexture("images/Button01.png");
    }

    TabBar::~TabBar()
    {
        // TODO Auto-generated destructor stub
    }
    void TabBar::render(core::Renderer *pRender)
    {
        //render tabbar
        graphics::Rect displayRect;
        if (getParent() != nullptr)
        {
            displayRect = getParent()->displayRect();
        }
        int index = 0;
        SDL_Color color = {198, 197, 197, 255};
        int tabX = static_cast<int>(displayRect.x) + 16;
        int taby = static_cast<int>(displayRect.y) + 16;
        graphics::Rect tabRect;
        graphics::Rect tabBarRect;
        tabBarRect.x = tabX;

        for (auto &tab : tabs)
        {
            const std::string &title = tab->getTitle();
            //render tab
            int w = 0;
            int h = 0;
            getFont()->size(title, &w, &h);

            graphics::Rect backgroundRect;
            backgroundRect.x = tabX;
            backgroundRect.y = taby;
            backgroundRect.width = tabWidth + 25;
            backgroundRect.height = tabHeight;

            pRender->setDrawColor(12, 21, 24, 255);
            pRender->fillRect(backgroundRect);

            if (index == hoveredTab)
            {
                color = {133, 133, 133, 255};
            }
            else if (index == currentTab)
            {
                color = {255, 255, 255, 255};
            }
            else
            {
                color = {198, 197, 197, 255};
            }

            //left top corner

            btnTexture->render(pRender, tabX, taby, 9, 9, 0, 0);
            //left bottom corner
            btnTexture->render(pRender, tabX, taby + tabHeight - 9, 9, 9, 0, 20);

            btnTexture->render(pRender, tabX + tabWidth + 25 - 9, taby, 9, 9, 174, 0);
            btnTexture->render(pRender, tabX + tabWidth + 25 - 9, taby + tabHeight - 9, 9, 9, 174, 20);

            //tabX += 10; // offset
            //render title
            getFont()->render(pRender, title, color, tabX + 10, taby + 5);
            taby += tabHeight + 10;
            index++;
        } /*
    tabBarRect.width = getWidth() - 5;
    pRender->setDrawColor(255, 255, 255, 255);
    pRender->drawRect(tabBarRect);*/

        pRender->setDrawColor(93, 103, 108, 255);
        utils::Vector2 start(displayRect.x + 183, displayRect.y + 16);
        utils::Vector2 end(displayRect.x + 183, displayRect.y + getHeight() - 16);
        pRender->drawLine(start, end);

        if (currentTab != -1)
        {
            tabs[currentTab]->render(pRender);
        }
    }
    void TabBar::handleEvents(core::Input *pInput)
    {
        if (currentTab != -1)
        {
            tabs[currentTab]->handleEvents(pInput);
            //handle tabbar events
        }

        hoveredTab = -1;
        graphics::Rect displayRect;
        if (getParent() != nullptr)
        {
            displayRect = getParent()->displayRect();
        }
        int tabX = static_cast<int>(displayRect.x) + 16;
        int taby = static_cast<int>(displayRect.y) + 16;
        graphics::Rect tabRect;
        int index = 0;
        for (auto &tab : tabs)
        {
            std::string title = tab->getTitle();
            //render tab
            int w = 0;
            int h = 0;
            getFont()->size(title, &w, &h);

            //render background
            tabRect.x = tabX;
            tabRect.y = taby;
            tabRect.width = tabWidth + 20;
            tabRect.height = tabHeight + 6;

            if (tabRect.intersects(pInput->getMousePostion()))
            {
                if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
                {
                    currentTab = index;
                }
                //hover
                hoveredTab = index;
            }

            tabX += 10; // offset
            taby += tabHeight + 10;
            index++;
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
        if (it != tabs.end())
        {
            tabs.erase(it);
            currentTab = 0;
        }
    }

    graphics::Rect TabBar::displayRect()
    {
        graphics::Rect r;
        r.x = getX() + 183;
        r.y = getY() + 20;
        if (getParent() != nullptr)
        {
            r.x += getParent()->displayRect().x;
            r.y += getParent()->displayRect().y;
        }
        return r;
    }

    graphics::Rect TabBar::eventRect()
    {
        graphics::Rect r;
        r.x = getX() + 183;
        r.y = getY() + 20;
        if (getParent() != nullptr)
        {
            r.x += getParent()->eventRect().x;
            r.y += getParent()->eventRect().y;
        }
        return r;
    }

    void TabBar::setCurrentTab(std::shared_ptr<Tab> tab)
    {
        auto it = std::find(tabs.begin(), tabs.end(), tab);
        currentTab = std::distance(tabs.begin(), it);
    }

} /* namespace UI */
