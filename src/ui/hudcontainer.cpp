#include "hudcontainer.h"
#include "translate.h"
#include <engine/core/gamewindow.h>
#include <engine/graphics/TextureManager.h>
#include <engine/ui/Button.h>
#include <engine/ui/StringHint.h>
#include <engine/ui/iconbutton.h>
#include <engine/utils/color.h>
#include <engine/utils/string.h>
namespace UI
{
    HUDContainer::HUDContainer(UpdateThread *updateThread, const std::shared_ptr<world::GameState> &gameState, UI::BuildWindow *buildWindow, UI::ResearchWindow *researchWindow)
        : updateThread(updateThread), gameState(gameState), buildWindow(buildWindow), researchWindow(researchWindow)
    {
        glyphText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);
        uiText = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);
        initUI();
    }

    void HUDContainer::render(core::Renderer *renderer)
    {

        //render time
        int xLeft = 570;
        int yLeft = 10;
        SDL_Color color = {100, 200, 0, 255};

        glyphText->render(renderer, "\uf017", color, xLeft, yLeft);
        xLeft += 25;
        auto format = gameState->getTime().format();
        uiText->render(renderer, format, color, xLeft, yLeft + 3);
        UI::Container::render(renderer);
    }

    void HUDContainer::initUI()
    {
        playButton = std::make_shared<UI::Button>();
        playButton->setFont("fonts/fa-solid-900.ttf", 20);
        playButton->setLabel("\uf04b");
        playButton->setColor(utils::color::WHITE);
        playButton->setPos(700, 0);
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
        pauseButton->setPos(730, 0);
        pauseButton->setColor(utils::color::WHITE);
        pauseButton->setBorderless(true);
        pauseButton->connect(UI::Button::buttonClickCallback(), [&]
                             { updateThread->pause(); });
        addObject(pauseButton);

        doubleSpeed = std::make_shared<UI::Button>();
        doubleSpeed->setFont("fonts/fa-solid-900.ttf", 20);
        doubleSpeed->setLabel("\uf04e");
        doubleSpeed->setPos(760, 0);
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
        fullSpeed->setPos(790, 0);
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

        xLeft += 150;
        researchButton = std::make_shared<UI::IconButton>();
        researchButton->setFont("fonts/arial.ttf", 12);
        researchButton->setLabel("");
        researchButton->setIconText("\uf0c3");
        researchButton->setColor(utils::color::WHITE);
        researchButton->setPos(xLeft, yLeft);
        researchButton->setBorderless(true);
        researchButton->connect("buttonClick", [&]()
                                {
                                    auto rect = researchWindow->displayRect();
                                    int width = core::GameWindow::Instance().getWidth();
                                    int height = core::GameWindow::Instance().getHeight();
                                    researchWindow->setPos(width / 2 - (rect.width / 2), height / 2 - (rect.height / 2));
                                    researchWindow->setVisible(true);
                                });
        addObject(cashButton);
        addObject(profitButton);
        addObject(researchButton);

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
        auto researchQueue = gameState->getPlayer()->getResearchQueue();
        if (researchQueue.size() == 0)
        {
            researchButton->setLabel(_("No active research"));
        }
        else
        {
            int resPerMonth = (gameState->getPlayer()->getResearchPerMonth() == 0) ? 1 : gameState->getPlayer()->getResearchPerMonth();
            auto researchTime = researchQueue.at(0)->getCurrentCosts() / resPerMonth;
            researchButton->setLabel(utils::string_format(u8"%s (%i months)", researchQueue.at(0)->getLocalisedName(), researchTime));
        }

        //cashButton->getHint()->setHintText(utils::string_format("Income Balance\n Cash: %'.2f € \nProfit: %'.2f €", gameState->getPlayer()->getCash(), gameState->getPlayer()->getProfit()));
        std::string hintText = "Income Balance\n";
        hintText += utils::string_format("Cash:   %.2f €", gameState->getPlayer()->getCash()) + "\n";
        hintText += utils::string_format("Income: %.2f €", gameState->getPlayer()->getIncome()) + "\n";
        hintText += utils::string_format("Costs:  %.2f €", gameState->getPlayer()->getCosts()) + "\n";
        hintText += utils::string_format("Profit: %.2f €", gameState->getPlayer()->getProfit());
        cashButton->getHint()->setHintText(hintText);
    }
}