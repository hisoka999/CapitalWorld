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

    std::sort(std::begin(objects),std::end(objects),[](Object* a,Object* b) -> bool
    {
        return a->getRenderOrder() < b->getRenderOrder();
    });
}
void Container::removeObject(Object* obj)
{
    auto it = std::find(objects.begin(),objects.end(),obj);
    if(it != objects.end())
        objects.erase(it);
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
