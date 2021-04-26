#include "hudcontainer.h"
#include <engine/ui/Button.h>
#include <engine/ui/iconbutton.h>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/color.h>
#include <engine/utils/string.h>

namespace UI
{
    HUDContainer::HUDContainer(UpdateThread *updateThread, const std::shared_ptr<world::GameState> &gameState, UI::BuildWindow *buildWindow)
        : updateThread(updateThread), gameState(gameState), buildWindow(buildWindow)
    {
        glyphText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);
        uiText = graphics::TextureManager::Instance().loadFont("fonts/Audiowide-Regular.ttf", 12);
        initUI();
    }

    void HUDContainer::render(core::Renderer *renderer)
    {
        std::time_t tmpTime = std::chrono::system_clock::to_time_t(
            gameState->getTime());

        const char date_time_format[] = "%d.%m.%Y";

        char time_str[100];

        std::strftime(time_str, 100, date_time_format,
                      std::localtime(&tmpTime));

        //render time
        int xLeft = 320;
        int yLeft = 10;
        SDL_Color color = {100, 200, 0, 255};

        glyphText->render(renderer, "\uf017", color, xLeft, yLeft);
        xLeft += 25;
        uiText->render(renderer, std::string(time_str), color, xLeft, yLeft + 3);
        UI::Container::render(renderer);
    }

    void HUDContainer::initUI()
    {
        playButton = std::make_shared<UI::Button>();
        playButton->setFont("fonts/fa-solid-900.ttf", 20);
        playButton->setLabel("\uf04b");
        playButton->setColor(utils::color::WHITE);
        playButton->setPos(450, 0);
        playButton->setBorderless(true);
        playButton->connect(UI::Button::buttonClickCallback(), [&] {
            updateThread->start();
            updateThread->setSpeed(300);
        });
        addObject(playButton);
        pauseButton = std::make_shared<UI::Button>();
        pauseButton->setFont("fonts/fa-solid-900.ttf", 20);
        pauseButton->setLabel("\uf04c");
        pauseButton->setPos(480, 0);
        pauseButton->setColor(utils::color::WHITE);
        pauseButton->setBorderless(true);
        pauseButton->connect(UI::Button::buttonClickCallback(), [&] {
            updateThread->pause();
        });
        addObject(pauseButton);

        doubleSpeed = std::make_shared<UI::Button>();
        doubleSpeed->setFont("fonts/fa-solid-900.ttf", 20);
        doubleSpeed->setLabel("\uf04e");
        doubleSpeed->setPos(510, 0);
        doubleSpeed->setBorderless(true);
        doubleSpeed->setColor(utils::color::WHITE);
        doubleSpeed->connect(UI::Button::buttonClickCallback(), [&] {
            updateThread->start();
            updateThread->setSpeed(100);
        });
        addObject(doubleSpeed);

        int xLeft = 50;
        int yLeft = 0;
        SDL_Color color = {255, 255, 0, 255};
        //cash
        cashButton = std::make_shared<UI::IconButton>();
        cashButton->setFont("fonts/arial.ttf", 12);
        cashButton->setLabel("");
        cashButton->setIconText("\uf51e");
        cashButton->setColor(utils::color::WHITE);
        cashButton->setPos(xLeft, yLeft);
        cashButton->setBorderless(true);
        xLeft += 170;
        profitButton = std::make_shared<UI::IconButton>();
        profitButton->setFont("fonts/arial.ttf", 12);
        profitButton->setLabel("");
        profitButton->setIconText("\uf0d6");
        profitButton->setColor(utils::color::WHITE);
        profitButton->setPos(xLeft, yLeft);
        profitButton->setBorderless(true);
        addObject(cashButton);
        addObject(profitButton);

        buildButton = std::make_shared<UI::IconButton>();
        buildButton->setIconText("\uf0c9");

        buildButton->setColor(utils::color::WHITE);
        buildButton->setFont("fonts/arial.ttf", 12);
        buildButton->setLabel(" ");
        buildButton->setPos(10, yLeft);
        buildButton->setBorderless(true);
        buildButton->connect("buttonClick", [&]() {
            std::cout << "build button clicked" << std::endl;
            buildWindow->setVisible(true);
        });
        addObject(buildButton);
    }

    void HUDContainer::update()
    {
        cashButton->setLabel(utils::string_format("%'.2f €", gameState->getPlayer()->getCash()));
        profitButton->setLabel(utils::string_format("%'.2f €", gameState->getPlayer()->getProfit()));
    }
}