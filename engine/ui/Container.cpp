#include "engine/ui/Container.h"

namespace UI
{

Container::Container()
{
    //ctor
}

Container::~Container()
{
    objects.clear();

}

void Container::addObject(Object* obj)
{
    objects.push_back(obj);
}
void Container::render(core::Renderer *pRender, graphics::Texture *pTexture)
{
    ObjectList::iterator it;
    for (it = objects.begin(); it != objects.end(); ++it)
    {
        (*it)->render(pRender, pTexture);
    }
}
void Container::handleEvents(core::Input *pInput)
{
    ObjectList::iterator it;
    for (it = objects.begin(); it != objects.end(); ++it)
    {
        (*it)->handleEvents(pInput);
    }
}
void Container::clear()
{
    objects.clear();
}

} // namespace UI
