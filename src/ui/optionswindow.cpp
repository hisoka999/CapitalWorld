#include "optionswindow.h"
#include <engine/ui/Button.h>
#include <engine/ui/layout/GridLayout.h>
#include <engine/core/SceneManager.h>
namespace UI
{
    OptionsWindow::OptionsWindow(int x, int y) : UI::Window(x, y, 200, 300)
    {

        setTitle("Options");
        mainArea = std::make_shared<UI::ScrollArea>(150, 250, this);
        mainArea->setPos(5, 5);
        addObject(mainArea);

        auto layout = std::make_shared<UI::layout::GridLayout>(mainArea.get(), 1);
        layout->setPadding(utils::Vector2(20, 10));

        auto loadButton = std::make_shared<UI::Button>(mainArea.get());
        loadButton->setFont("fonts/arial.ttf", 12);
        loadButton->setLabel("Load Game");
        mainArea->addObject(loadButton);

        auto saveButton = std::make_shared<UI::Button>(mainArea.get());
        saveButton->setFont("fonts/arial.ttf", 12);
        saveButton->setLabel("Save Game");
        mainArea->addObject(saveButton);

        auto optionsButton = std::make_shared<UI::Button>(mainArea.get());
        optionsButton->setFont("fonts/arial.ttf", 12);
        optionsButton->setLabel("Options");
        mainArea->addObject(optionsButton);
        optionsButton->connect(UI::Button::buttonClickCallback(), [&]
                               {
                                   settingsWindow.setPos(300, 300);
                                   settingsWindow.setVisible(true);
                               });

        auto mainMenuButton = std::make_shared<UI::Button>(mainArea.get());
        mainMenuButton->setFont("fonts/arial.ttf", 12);
        mainMenuButton->setLabel("Back to main");
        mainArea->addObject(mainMenuButton);
        mainMenuButton->connect(UI::Button::buttonClickCallback(), []
                                { core::SceneManager::Instance().setCurrentScene("main"); });

        graphics::Rect bounds = {10, 10, float(mainArea->getWidth()), float(mainArea->getHeight())};

        layout->updateLayout(bounds);
        mainArea->reset();
    }

    OptionsWindow::~OptionsWindow()
    {
    }

    void OptionsWindow::render(core::Renderer *pRender)
    {
        UI::Window::render(pRender);
        settingsWindow.render(pRender);
    }

    void OptionsWindow::handleEvents(core::Input *pInput)
    {
        UI::Window::handleEvents(pInput);
        settingsWindow.handleEvents(pInput);
    }

}