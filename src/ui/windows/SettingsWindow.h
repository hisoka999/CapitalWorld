#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <engine/ui/Window.h>
#include <engine/ui/Object.h>
#include <engine/ui/Checkbox.h>
#include <engine/ui/Button.h>
#include <engine/ui/ComboBox.h>
#include <engine/core/gamewindow.h>
#include <engine/ui/scrollarea.h>
#include <engine/ui/TabBar.h>
#include <vector>

struct DisplayMode
{
    uint32_t width;
    uint32_t height;

    std::string toString()
    {
        return std::to_string(width) + " x " + std::to_string(height);
    }
};

class SettingsWindow : public UI::Window
{
public:
    /** Default constructor */
    SettingsWindow();

    /** Default destructor */
    virtual ~SettingsWindow();

protected:
    void closeWindow();

private:
    std::shared_ptr<UI::TabBar> tabBar;
    std::shared_ptr<UI::ComboBox<core::FullScreenMode>> fullscreen;
    std::shared_ptr<UI::Checkbox> vsync;
    std::shared_ptr<UI::Button> saveButton;
    std::shared_ptr<UI::Button> cancelButton;
    std::shared_ptr<UI::ComboBox<DisplayMode>> resolutions;
    int display_in_use = 0;
    std::shared_ptr<graphics::Text> uiText;
    std::shared_ptr<graphics::Text> uiIconText;
    std::vector<DisplayMode> displayModes;
};

#endif // SETTINGSWINDOW_H
