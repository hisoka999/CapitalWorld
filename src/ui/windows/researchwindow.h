#ifndef RESEARCHWINDOW_H
#define RESEARCHWINDOW_H

#include "ui/researchbutton.h"
#include "world/company.h"
#include <engine/ui/Window.h>
#include <engine/ui/scrollarea.h>
#include <world/gamestate.h>

namespace UI
{

    class ResearchWindow : public UI::Window
    {
    public:
        ResearchWindow(const std::shared_ptr<world::GameState> &gameState);

    private:
        void buildResearchTree(int x, int y, const std::shared_ptr<Research> &baseResearch, const std::vector<std::shared_ptr<Research>> &researchList);
        //std::vector<UI::ResearchButton> researchButtons;
        std::shared_ptr<UI::ScrollArea> scrollArea;
        std::shared_ptr<world::GameState> gameState;
    };
};
#endif // RESEARCHWINDOW_H
