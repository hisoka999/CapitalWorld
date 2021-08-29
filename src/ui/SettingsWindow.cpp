#include "SettingsWindow.h"
#include "../translate.h"
#include <engine/graphics/TextureManager.h>
#include <engine/ui/layout/GridLayout.h>
#include <engine/ui/Label.h>
#include "magic_enum.hpp"
#include <functional>
SettingsWindow::SettingsWindow()
    : UI::Window(50, 50, 520, 400)
{
    tabBar = std::make_shared<UI::TabBar>(this);
    tabBar->setPos(5, 5);
    tabBar->setWidth(400);
    tabBar->setHeight(300);
    addObject(tabBar);
    auto generalTab = std::make_shared<UI::Tab>(tabBar.get(), _("General"));
    auto graphicsTab = std::make_shared<UI::Tab>(tabBar.get(), _("Graphics"));
    auto soundTab = std::make_shared<UI::Tab>(tabBar.get(), _("Sound"));
    auto controlsTab = std::make_shared<UI::Tab>(tabBar.get(), _("Controls"));
    tabBar->addTab(generalTab);
    tabBar->addTab(graphicsTab);
    tabBar->addTab(soundTab);
    tabBar->addTab(controlsTab);

    auto layout = std::make_shared<UI::layout::GridLayout>(graphicsTab.get(), 2);
    layout->setPadding(utils::Vector2(20, 10));

    uiText = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);

    setFont(uiText.get());
    this->fullscreen = std::make_shared<UI::ComboBox<core::FullScreenMode>>(graphicsTab.get());
    vsync = std::make_shared<UI::Checkbox>(graphicsTab.get());
    this->cancelButton = std::make_shared<UI::Button>(this);
    this->saveButton = std::make_shared<UI::Button>(this);
    auto comboboxLanguage = std::make_shared<UI::ComboBox<Language>>(generalTab.get());

    saveButton->setLabel(_("Save"));
    saveButton->setPos(30, 300);
    saveButton->connect(UI::Button::buttonClickCallback(), [=]()
                        {
                            auto settings = core::GameWindow::Instance().getSettings();
                            settings->setAttrI("Base", "Fullscreen", int(fullscreen->getSelectionText()));
                            core::GameWindow::Instance().setFullScreen(fullscreen->getSelectionText());
                            settings->setAttrB("Base", "VSync", vsync->isChecked());
                            int i = resolutions->getSelection();

                            settings->setAttrI("Base", "Height", displayModes[i].height);
                            settings->setAttrI("Base", "Width", displayModes[i].width);
                            settings->setAttr("Base", "Lang", std::string(magic_enum::enum_name(comboboxLanguage->getSelectionText())));

                            settings->write();
                            closeWindow();
                        });

    cancelButton->setLabel(_("Cancel"));
    cancelButton->setPos(200, 300);
    fullscreen->setPos(30, 50);
    fullscreen->setHeight(25);
    //fullscreen->setText(_("Fullscreen"));
    setTitle(_("Settings"));
    resolutions = std::make_shared<UI::ComboBox<DisplayMode>>(graphicsTab.get());

    resolutions->setElementFunction([](DisplayMode val)
                                    { return val.toString(); });
    resolutions->setPos(30, 70);
    resolutions->setWidth(200);
    auto settings = core::GameWindow::Instance().getSettings();

    constexpr auto &modes = magic_enum::enum_values<core::FullScreenMode>();

    for (auto &value : modes)
    {
        fullscreen->addElement(value);
    }
    fullscreen->setElementFunction([](core::FullScreenMode val) -> std::string
                                   { return _(std::string(magic_enum::enum_name(val))); });

    fullscreen->setSelectionByText((core::FullScreenMode)settings->getValueI("Base", "Fullscreen"));
    vsync->setChecked(settings->getValueB("Base", "VSync"));
    vsync->setHeight(25);
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
    auto labelResolution = std::make_shared<UI::Label>(graphicsTab.get());
    labelResolution->setFont(uiText.get());
    labelResolution->setText(_("Resolution"));

    auto labelFullscreen = std::make_shared<UI::Label>(graphicsTab.get());
    labelFullscreen->setFont(uiText.get());
    labelFullscreen->setText(_("Fullscreen"));

    auto labelVSync = std::make_shared<UI::Label>(graphicsTab.get());
    labelVSync->setFont(uiText.get());
    labelVSync->setText(_("VSync"));

    // general tab
    auto labelLanguage = std::make_shared<UI::Label>(generalTab.get());
    labelLanguage->setFont(uiText.get());
    labelLanguage->setText(_("Language"));
    labelLanguage->setPos(5, 5);

    comboboxLanguage->setFont("fonts/arial.ttf", 14);
    // comboboxLanguage->connect("valueChanged", [&](Language size)
    //                           { worldSize = size; });
    constexpr auto &languages = magic_enum::enum_values<Language>();

    for (auto &value : languages)
    {
        comboboxLanguage->addElement(value);
    }
    Language lang = Localisation::Instance().getLang();
    if (!settings->getValue("Base", "Lang").empty())
    {
        lang = magic_enum::enum_cast<Language>(settings->getValue("Base", "Lang")).value();
    }
    comboboxLanguage->setSelectionByText(lang);

    comboboxLanguage->setPos(105, 5);
    comboboxLanguage->setWidth(200);
    comboboxLanguage->setElementFunction([](Language val) -> std::string
                                         { return _(std::string(magic_enum::enum_name(val))); });

    generalTab->addObject(labelLanguage);
    generalTab->addObject(comboboxLanguage);

    graphicsTab->addObject(labelResolution);
    graphicsTab->addObject(resolutions);
    graphicsTab->addObject(labelFullscreen);
    graphicsTab->addObject(fullscreen);
    graphicsTab->addObject(labelVSync);
    graphicsTab->addObject(vsync);
    addObject(cancelButton);
    addObject(saveButton);

    graphics::Rect bounds = {10, 10, float(graphicsTab->getWidth()), float(graphicsTab->getHeight())};

    layout->updateLayout(bounds);
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
