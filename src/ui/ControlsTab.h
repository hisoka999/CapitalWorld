#pragma once

#include <memory>
#include <engine/utils/IniBase.h>
#include <engine/core/input.h>
#include <engine/ui/Tab.h>
namespace UI
{
    class ControlsTab : public UI::Tab
    {
    private:
        core::Input *input;
        std::shared_ptr<utils::IniBase> settings;

    public:
        ControlsTab(Object *parent, const std::string &title, core::Input *input, std::shared_ptr<utils::IniBase> &settings);
        ~ControlsTab();
    };

} // namespace UI
