#ifndef RESEARCHBUTTON_H
#define RESEARCHBUTTON_H

#include "world/research.h"
#include <engine/ui/Object.h>
#include "world/gamestate.h"
class Player;

namespace UI
{

    class ResearchButton : public UI::Object
    {
    public:
        ResearchButton(const std::shared_ptr<Research> &research, const std::shared_ptr<world::GameState> &gameState, UI::Object *parent = nullptr);
        virtual ~ResearchButton();

        void render(core::Renderer *pRender);

        void handleEvents(core::Input *pInput);

    private:
        std::shared_ptr<Research> research;
        std::shared_ptr<world::GameState> gameState;

        std::shared_ptr<graphics::Text> iconFont;
        std::shared_ptr<graphics::Text> textFont;

        std::shared_ptr<graphics::Text> smallIconFont;
        std::shared_ptr<graphics::Text> smallTextFont;

        bool hovered = false;
        bool clicked = false;
    };

}

#endif // RESEARCHBUTTON_H
