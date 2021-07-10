#include "hudcontainer.h"
#include <engine/ui/Button.h>
#include <engine/ui/iconbutton.h>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/color.h>
#include <engine/utils/string.h>
#include <engine/ui/StringHint.h>

namespace UI
{
    HUDContainer::HUDContainer(UpdateThread *updateThread, const std::shared_ptr<world::GameState> &gameState, UI::BuildWindow *buildWindow)
        : updateThread(updateThread), gameState(gameState), buildWindow(buildWindow)
    {
        glyphText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);
        uiText = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);
        initUI();
    }

    void HUDContainer::render(core::Renderer *renderer)
    {
        std::time_t tmpTime = std::chrono::system_clock::to_time_t(
            gameState->getTime());

        const char date_time_format[] = "%d.%m.%Y";
        // time(&tmpTime);
        char time_str[100];
        std::tm *timeinfo = std::localtime(&tmpTime);

        std::strftime(time_str, 100, date_time_format, timeinfo);

        //render time
        int xLeft = 370;
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
        playButton->setPos(500, 0);
        playButton->setBorderless(true);
        playButton->connect(UI::Button::buttonClickCallback(), [&]
                            {
                                updateThread->start();
                                updateThread->setSpeed(300);
                            });
        addObject(playButton);
        pauseButton = std::make_shared<UI::Button>();
        pauseButton->setFont("fonts/fa-solid-900.ttf", 20);
        pauseButton->setLabel("\uf04c");
        pauseButton->setPos(530, 0);
        pauseButton->setColor(utils::color::WHITE);
        pauseButton->setBorderless(true);
        pauseButton->connect(UI::Button::buttonClickCallback(), [&]
                             { updateThread->pause(); });
        addObject(pauseButton);

        doubleSpeed = std::make_shared<UI::Button>();
        doubleSpeed->setFont("fonts/fa-solid-900.ttf", 20);
        doubleSpeed->setLabel("\uf04e");
        doubleSpeed->setPos(560, 0);
        doubleSpeed->setBorderless(true);
        doubleSpeed->setColor(utils::color::WHITE);
        doubleSpeed->connect(UI::Button::buttonClickCallback(), [&]
                             {
                                 updateThread->start();
                                 updateThread->setSpeed(100);
                             });
        addObject(doubleSpeed);

        fullSpeed = std::make_shared<UI::Button>();
        fullSpeed->setFont("fonts/fa-solid-900.ttf", 20);
        fullSpeed->setLabel("\uf050");
        fullSpeed->setPos(590, 0);
        fullSpeed->setBorderless(true);
        fullSpeed->setColor(utils::color::WHITE);
        fullSpeed->connect(UI::Button::buttonClickCallback(), [&]
                           {
                               updateThread->start();
                               updateThread->setSpeed(50);
                           });
        addObject(fullSpeed);

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
        cashButton->setHint(std::make_shared<UI::StringHint>());

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
        buildButton->connect("buttonClick", [&]()
                             {
                                 std::cout << "build button clicked" << std::endl;
                                 buildWindow->setVisible(true);
                             });
        addObject(buildButton);
    }

    void HUDContainer::update()
    {
        cashButton->setLabel(utils::string_format(u8"%.2f €", gameState->getPlayer()->getCash()));
        profitButton->setLabel(utils::string_format(u8"%.2f €", gameState->getPlayer()->getProfit()));

        //cashButton->getHint()->setHintText(utils::string_format("Income Balance\n Cash: %'.2f € \nProfit: %'.2f €", gameState->getPlayer()->getCash(), gameState->getPlayer()->getProfit()));
        std::string hintText = "Income Balance\n";
        hintText += utils::string_format("Cash:   %.2f €", gameState->getPlayer()->getCash()) + "\n";
        hintText += utils::string_format("Income: %.2f €", gameState->getPlayer()->getIncome()) + "\n";
        hintText += utils::string_format("Costs:  %.2f €", gameState->getPlayer()->getCosts()) + "\n";
        hintText += utils::string_format("Profit: %.2f €", gameState->getPlayer()->getProfit());
        cashButton->getHint()->setHintText(hintText);
    }
}