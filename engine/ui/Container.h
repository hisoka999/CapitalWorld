#ifndef UI_CONTAINER_H
#define UI_CONTAINER_H

#include "engine/core/input.h"
#include "engine/core/renderer.h"
#include "engine/ui/Object.h"
#include <memory>
#include <vector>

namespace UI
{
    typedef std::vector<std::shared_ptr<Object>> ObjectList;
    class Container
    {
    public:
        /** Default constructor */
        Container();
        /** Default destructor */
        virtual ~Container();
        void addObject(const std::shared_ptr<Object> &obj);
        void removeObject(const std::shared_ptr<Object> &obj);
        virtual void clear();
        virtual void render(core::Renderer *pRender);
        virtual void handleEvents(core::Input *pInput);
        size_t size();
        std::shared_ptr<Object> get(size_t pos);

    protected:
        ObjectList objects;

    private:
    };

} // namespace UI

#endif // UI_CONTAINER_H
