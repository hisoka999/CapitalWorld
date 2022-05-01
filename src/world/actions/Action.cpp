#include "Action.h"

namespace world
{
    namespace actions
    {
        std::shared_ptr<Action> Action::nextAction()
        {
            return m_action;
        }

        void Action::setNextAction(const std::shared_ptr<Action> &action)
        {
            m_action = action;
        }
    }
}