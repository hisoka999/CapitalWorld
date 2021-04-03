#include "engine/ui/Container.h"
#include <algorithm>

namespace UI
{

    Container::Container()
    {
        //ctor
    }

    Container::~Container()
    {
    }

    void Container::addObject(const std::shared_ptr<Object> &obj)
    {
        objects.push_back(obj);

        // std::sort(std::begin(objects), std::end(objects), [](const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b) -> bool {
        //     return a->getRenderOrder() < b->getRenderOrder();
        // });
    }
    void Container::removeObject(const std::shared_ptr<Object> &obj)
    {
        auto it = std::find(objects.begin(), objects.end(), obj);
        if (it != objects.end())
            objects.erase(it);
    }
    void Container::render(core::Renderer *pRender)
    {
        ObjectList::iterator it;
        for (it = objects.begin(); it != objects.end(); ++it)
        {
            (*it)->render(pRender);
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

    size_t Container::size()
    {
        return objects.size();
    }

    std::shared_ptr<Object> Container::get(size_t pos)
    {
        return objects.at(pos);
    }
    void Container::clear()
    {
        objects.clear();
    }

} // namespace UI
