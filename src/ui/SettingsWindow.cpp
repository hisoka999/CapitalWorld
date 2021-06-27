#include "SettingsWindow.h"
#include "../translate.h"
#include <engine/graphics/TextureManager.h>
#include <engine/ui/layout/GridLayout.h>
#include <engine/ui/Label.h>

#include <functional>
SettingsWindow::SettingsWindow()
    : UI::Window(50, 50, 490, 300)
{
    mainArea = std::make_shared<UI::ScrollArea>(450, 250, this);
    mainArea->setPos(5, 5);
    addObject(mainArea);

    auto layout = std::make_shared<UI::layout::GridLayout>(mainArea.get(), 2);
    layout->setPadding(utils::Vector2(10, 10));

    uiText = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);

    setFont(uiText.get());
    this->fullscreen = std::make_shared<UI::Checkbox>(mainArea.get());
    this->cancelButton = std::make_shared<UI::Button>(mainArea.get());
    this->saveButton = std::make_shared<UI::Button>(mainArea.get());

    saveButton->setLabel(_("Save"));
    saveButton->setPos(30, 200);
    saveButton->connect(UI::Button::buttonClickCallback(), [=]()
                        {
                            auto settings = core::GameWindow::Instance().getSettings();
                            settings->setAttrB("Base", "Fullscreen", fullscreen->isChecked());
                            int i = resolutions->getSelection();

                            settings->setAttrI("Base", "Height", displayModes[i].height);
                            settings->setAttrI("Base", "Width", displayModes[i].width);

                            settings->write();
                            closeWindow();
                        });

    cancelButton->setLabel(_("Cancel"));
    cancelButton->setPos(200, 200);
    fullscreen->setPos(30, 50);
    //fullscreen->setText(_("Fullscreen"));
    setTitle(_("Settings"));
    resolutions = std::make_shared<UI::ComboBox<DisplayMode>>(mainArea.get());

    resolutions->setElementFunction([](DisplayMode val)
                                    { return val.toString(); });
    resolutions->setPos(30, 70);
    resolutions->setWidth(200);
    auto settings = core::GameWindow::Instance().getSettings();
    fullscreen->setChecked(settings->getValueB("Base", "Fullscreen"));
    cancelButton->connect("buttonClick", [&]()
                          { closeWindow(); });
    int screenWidth = settings->getValueI("Base", "Width");
    int screenHeight = settings->getValueI("Base", "Height");

    /* Get available fullscreen/hardware modes */

    int display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1)
    {
        throw SDLException("SDL_GetNumDisplayModes");
    }
    SDL_DisplayMode mode;
    for (int i = 0; i < display_mode_count; ++i)
    {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0)
        {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            return;
        }
        DisplayMode displayMode;
        displayMode.width = mode.w;
        displayMode.height = mode.h;
        bool exists = false;
        for (auto &t : displayModes)
        {
            if (t.width == displayMode.width && t.height == displayMode.height)
                exists = true;
        }
        if (!exists)
        {
            std::cout << "mode: " << std::to_string(mode.w) + " x " + std::to_string(mode.h) << std::endl;

            std::cout << "mode: " << displayMode.toString() << std::endl;
            displayModes.push_back(displayMode);
            resolutions->addElement(displayMode);
            if (mode.w == screenWidth && mode.h == screenHeight)
                resolutions->setSelection(displayModes.size() - 1);
        }
        //FIXME distinct mode list
    }
    auto labelResolution = std::make_shared<UI::Label>(mainArea.get());
    labelResolution->setFont(uiText.get());
    labelResolution->setText(_("Resolution"));

    auto labelFullscreen = std::make_shared<UI::Label>(mainArea.get());
    labelFullscreen->setFont(uiText.get());
    labelFullscreen->setText(_("Fullscreen"));

    mainArea->addObject(labelResolution);
    mainArea->addObject(resolutions);
    mainArea->addObject(labelFullscreen);
    mainArea->addObject(fullscreen);
    mainArea->addObject(cancelButton);
    mainArea->addObject(saveButton);

    graphics::Rect bounds = {10, 10, mainArea->getWidth(), mainArea->getHeight()};

    layout->updateLayout(bounds);
    mainArea->reset();
}

SettingsWindow::~SettingsWindow()
{
    uiText = nullptr;
    uiIconText = nullptr;
}
void SettingsWindow::closeWindow()
{
    this->setVisible(false);
}
