#include "engine/ui/windowmanager.h"
#include <algorithm>
#include "engine/ui/Window.h"

namespace UI {

WindowManager::WindowManager(graphics::Texture* baseTexture) :
        texture(baseTexture) {
    addWindow (&messageDialog);
}

WindowManager::~WindowManager() {
    //dtor
}

void WindowManager::addContainer(Container *c) {
    containers.push_back(c);
}
void WindowManager::addWindow(Window *win) {
    win->setFont(&font);
    containers.push_back(win);
}

void WindowManager::render(core::Renderer* ren) {
    for_each(containers.begin(), containers.end(), [&](Container* c) {
        c->render(ren,texture);
    });
}

void WindowManager::handleInput(core::Input *inp) {
    for_each(containers.begin(), containers.end(), [&](Container* c) {
        c->handleEvents(inp);
    });
}

bool WindowManager::isWindowOpen() {
    for (auto container : containers) {
        Window* win = (Window*) container;
        if (!win)
            continue;
        if (win->getVisible())
            return true;
    }
    return false;
}

} // namespace UI
