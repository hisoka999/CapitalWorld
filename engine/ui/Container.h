#ifndef UI_CONTAINER_H
#define UI_CONTAINER_H

#include "engine/ui/Object.h"
#include <list>
#include "engine/core/input.h"
#include "engine/core/renderer.h"

namespace UI
{
typedef std::list<Object*> ObjectList;
class Container
{
public:
    /** Default constructor */
    Container();
    /** Default destructor */
    virtual ~Container();
    void addObject(Object* obj);
    virtual void clear();
    virtual void render(core::Renderer *pRender, graphics::Texture *pTexture);
    virtual void handleEvents(core::Input *pInput);
protected:
    ObjectList objects;
private:

};

} // namespace UI

#endif // UI_CONTAINER_H
