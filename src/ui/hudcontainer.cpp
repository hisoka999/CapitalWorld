#include "hudcontainer.h"
#include "translate.h"
#include "ui/ResearchHint.h"
#include <engine/core/gamewindow.h>
#include <engine/graphics/TextureManager.h>
#include <engine/ui/Button.h>
#include <engine/ui/StringHint.h>
#include <engine/ui/iconbutton.h>
#include <engine/utils/color.h>
#include <engine/utils/string.h>

namespace UI
{
    HUDContainer::HUDContainer(UpdateThread *updateThread, world::AIThread *aiThread, const std::shared_ptr<world::GameState> &gameState, UI::BuildWindow *buildWindow, UI::ResearchWindow *researchWindow, UI::PlayerWindow *playerWindow)
        : updateThread(updateThread), aiThread(aiThread), gameState(gameState), buildWindow(buildWindow), researchWindow(researchWindow), playerWindow(playerWindow)
    {
        glyphText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);
        uiText = graphics::TextureManager::Instance().loadFont("fonts/arial.ttf", 12);
        initUI();
    }

    void HUDContainer::initUI()
    {

        int xLeft = 50;
        int yLeft = 0;

        companyButton = std::make_shared<UI::IconButton>();
        companyButton->setFont("fonts/arial.ttf", 12);
        companyButton->setLabel(gameState->getPlayer()->getName());
        companyButton->setIconText("\uf1ad");
        companyButton->setColor(utils::color::WHITE);
        companyButton->setPos(xLeft, yLeft);
        companyButton->setBorderless(true);
        companyButton->connect("buttonClick", [&]()
                               {
                                   auto rect = playerWindow->displayRect();
                                   int width = core::GameWindow::Instance().getWidth();
                                   int height = core::GameWindow::Instance().getHeight();
                                   playerWindow->setPos(width / 2 - (rect.width / 2), height / 2 - (rect.height / 2));
                                   playerWindow->setVisible(true); });
        xLeft += 170;

        // cash
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
                                    researchWindow->setVisible(true); });
        xLeft += 200;
        addObject(companyButton);
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
                                 buildWindow->setVisible(true); });
        addObject(buildButton);

        timeButton = std::make_shared<UI::IconButton>();
        timeButton->setFont("fonts/arial.ttf", 12);
        timeButton->setIconText("\uf017");
        timeButton->setLabel(gameState->getTime().format());
        timeButton->setColor(utils::color::GREEN);
        timeButton->setPos(xLeft, 0);
        timeButton->setBorderless(true);
        // timeButton->disable();
        addObject(timeButton);
        xLeft += 100;
        playButton = std::make_shared<UI::Button>();
        playButton->setFont("fonts/fa-solid-900.ttf", 20);
        playButton->setLabel("\uf04b");
        playButton->setColor(utils::color::WHITE);
        playButton->setPos(xLeft, 0);
        playButton->setBorderless(true);
        playButton->setToggleAllowed(true);
        playButton->setToggled(true);
        xLeft += 30;
        addObject(playButton);
        pauseButton = std::make_shared<UI::Button>();
        pauseButton->setFont("fonts/fa-solid-900.ttf", 20);
        pauseButton->setLabel("\uf04c");
        pauseButton->setPos(xLeft, 0);
        pauseButton->setColor(utils::color::WHITE);
        pauseButton->setBorderless(true);
        pauseButton->setToggleAllowed(true);

        addObject(pauseButton);
        xLeft += 30;

        doubleSpeed = std::make_shared<UI::Button>();
        doubleSpeed->setFont("fonts/fa-solid-900.ttf", 20);
        doubleSpeed->setLabel("\uf04e");
        doubleSpeed->setPos(xLeft, 0);
        doubleSpeed->setBorderless(true);
        doubleSpeed->setColor(utils::color::WHITE);
        doubleSpeed->setToggleAllowed(true);

        addObject(doubleSpeed);
        xLeft += 30;

        fullSpeed = std::make_shared<UI::Button>();
        fullSpeed->setFont("fonts/fa-solid-900.ttf", 20);
        fullSpeed->setLabel("\uf050");
        fullSpeed->setPos(xLeft, 0);
        fullSpeed->setBorderless(true);
        fullSpeed->setColor(utils::color::WHITE);
        fullSpeed->setToggleAllowed(true);
        fullSpeed->connect(UI::Button::buttonClickCallback(), [&]
                           {
                                updateThread->start();
                                updateThread->setSpeed(50); 
                                aiThread->start();
                                aiThread->setSpeed(50);
                                playButton->setToggled(false);
                                pauseButton->setToggled(false);
                                fullSpeed->setToggled(true);
                                doubleSpeed->setToggled(false); });
        addObject(fullSpeed);

        doubleSpeed->connect(UI::Button::buttonClickCallback(), [&]
                             {
                                updateThread->start();
                                updateThread->setSpeed(100);
                                aiThread->start();
                                aiThread->setSpeed(100);
                                playButton->setToggled(false);
                                pauseButton->setToggled(false);
                                fullSpeed->setToggled(false);
                                doubleSpeed->setToggled(true); });

        playButton->connect(UI::Button::buttonClickCallback(), [&]
                            {
                                updateThread->start();
                                updateThread->setSpeed(300);
                                aiThread->start();
                                aiThread->setSpeed(300);
                                playButton->setToggled(true);
                                pauseButton->setToggled(false);
                                fullSpeed->setToggled(false);
                                doubleSpeed->setToggled(false); });

        pauseButton->connect(UI::Button::buttonClickCallback(), [&]
                             { 
                                updateThread->pause();
                                aiThread->pause();
                                playButton->setToggled(false);
                                pauseButton->setToggled(true);
                                fullSpeed->setToggled(false);
                                doubleSpeed->setToggled(false); });
    }

    void HUDContainer::update()
    {
        cashButton->setLabel(utils::string_format(u8"%.2f €", gameState->getPlayer()->getCash()));
        profitButton->setLabel(utils::string_format(u8"%.2f €", gameState->getPlayer()->getProfit()));
        auto researchQueue = gameState->getPlayer()->getResearchQueue();
        if (researchQueue.size() == 0)
        {
            researchButton->setLabel(_("No active research"));
            // researchHint = nullptr;
        }
        else
        {
            int resPerMonth = (gameState->getPlayer()->getResearchPerMonth() == 0) ? 1 : gameState->getPlayer()->getResearchPerMonth();
            auto researchTime = researchQueue.at(0)->getCurrentCosts() / resPerMonth;
            researchButton->setLabel(utils::string_format(u8"%s (%i months)", researchQueue.at(0)->getLocalisedName(), researchTime));

            auto pos = (researchButton->getHint() != nullptr) ? researchButton->getHint()->getPosition() : utils::Vector2(0.f, 0.f);
            auto researchHint = std::make_shared<UI::ResearchHint>(researchQueue.at(0));
            researchHint->setPosition(pos);
            researchButton->setHint(researchHint);
        }

        // cashButton->getHint()->setHintText(utils::string_format("Income Balance\n Cash: %'.2f € \nProfit: %'.2f €", gameState->getPlayer()->getCash(), gameState->getPlayer()->getProfit()));
        std::string hintText = "Income Balance\n";
        hintText += utils::string_format("Cash:   %.2f €", gameState->getPlayer()->getCash()) + "\n";
        hintText += utils::string_format("Income: %.2f €", gameState->getPlayer()->getIncome()) + "\n";
        hintText += utils::string_format("Costs:  %.2f €", gameState->getPlayer()->getCosts()) + "\n";
        hintText += utils::string_format("Profit: %.2f €", gameState->getPlayer()->getProfit());
        cashButton->getHint()->setHintText(hintText);

        timeButton->setLabel(gameState->getTime().format());
    }
}