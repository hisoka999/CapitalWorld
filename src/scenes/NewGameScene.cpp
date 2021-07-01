#include "NewGameScene.h"
#include <magic_enum.hpp>
#include <engine/core/SceneManager.h>
#include <engine/graphics/TextureManager.h>
#include <engine/ui/TextItem.h>
#include <engine/ui/ComboBox.h>
#include "../translate.h"
#include <string>
#include "scenes/worldscene.h"
#include "world/mapgenerator.h"

#include <random>
#include <engine/ui/layout/GridLayout.h>
#include <engine/utils/color.h>

namespace scenes
{
    NewGameScene::NewGameScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager) : core::Scene(pRenderer), sceneManager(pSceneManager)
    {

        const auto &viewPort = pRenderer->getViewPort();
        container = std::make_shared<UI::Container>();
        buttonList = std::make_shared<UI::Container>();
        auto layout = std::make_shared<UI::layout::GridLayout>(container.get(), 2);
        layout->setPadding(utils::Vector2(10, 10));

        auto startButton = std::make_shared<UI::Button>();
        startButton->setPos(viewPort.width - 150, viewPort.height - 50);
        startButton->setFont("fonts/Audiowide-Regular.ttf", 14);
        startButton->setLabel(_("Start Game"));

        startButton->connect(UI::Button::buttonClickCallback(), [&]()
                             { startGame(); });
        buttonList->addObject(startButton);

        auto backButton = std::make_shared<UI::Button>();
        backButton->setPos(viewPort.width - 250, viewPort.height - 50);
        backButton->setFont("fonts/Audiowide-Regular.ttf", 14);
        backButton->setLabel(_("Back"));

        buttonList->addObject(backButton);
        winMgr->addContainer(buttonList.get());

        //add comboboxes and textfields
        int yOffset = 30;
        int y = 300;

        auto nameLabel = std::make_shared<UI::Label>(nullptr);
        nameLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        nameLabel->setText(_("Playername: "));
        nameLabel->setPos(20, y);
        container->addObject(nameLabel);

        auto nameEdit = std::make_shared<UI::TextItem>(nullptr, 200, 25);
        nameEdit->setPos(200, y);
        nameEdit->setFont("fonts/Audiowide-Regular.ttf", 14);
        playerName = "Player";
        nameEdit->setText(playerName);

        nameEdit->connect("textChanged", [&](std::string data)
                          { playerName = data; });

        container->addObject(nameEdit);

        y += yOffset;
        auto seedLabel = std::make_shared<UI::Label>(nullptr);
        seedLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        seedLabel->setText(_("Seed: "));
        seedLabel->setPos(20, y);
        container->addObject(seedLabel);

        auto seedEdit = std::make_shared<UI::TextItem>(nullptr, 200, 25);
        seedEdit->setPos(200, y);
        seedEdit->setFont("fonts/Audiowide-Regular.ttf", 14);
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        seedEdit->setText(std::to_string(seed));
        seedEdit->connect("textChanged", [&](std::string data)
                          { seed = std::stoul(data); });

        container->addObject(seedEdit);

        y += yOffset;
        auto numberOfPlayersLabel = std::make_shared<UI::Label>(nullptr);
        numberOfPlayersLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        numberOfPlayersLabel->setText(_("Number of Companies: "));
        numberOfPlayersLabel->setPos(20, y);
        container->addObject(numberOfPlayersLabel);

        auto numberOfPlayersCombobox = std::make_shared<UI::ComboBox<int>>();
        numberOfPlayersCombobox->setFont("fonts/Audiowide-Regular.ttf", 14);
        numberOfPlayersCombobox->addElement(2);
        numberOfPlayersCombobox->addElement(4);
        numberOfPlayersCombobox->addElement(8);
        //numberOfPlayersCombobox->addElement(16);
        //numberOfPlayersCombobox->addElement(32);
        numberOfPlayersCombobox->setPos(200, y);
        numberOfPlayersCombobox->setSelectionByText(8);
        numberOfPlayersCombobox->setWidth(200);
        numberOfPlayersCombobox->setElementFunction([](int val)
                                                    { return std::to_string(val); });

        numberOfPlayersCombobox->connect("valueChanged", [&](int size)
                                         { numberOfCompanys = size; });
        container->addObject(numberOfPlayersCombobox);

        y += yOffset;
        auto systemSizeLabel = std::make_shared<UI::Label>(nullptr);
        systemSizeLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        systemSizeLabel->setText(_("Size of the System: "));
        systemSizeLabel->setPos(20, y);
        container->addObject(systemSizeLabel);

        auto systemSizeCombobox = std::make_shared<UI::ComboBox<WorldSize>>();
        systemSizeCombobox->setFont("fonts/Audiowide-Regular.ttf", 14);
        systemSizeCombobox->connect("valueChanged", [&](WorldSize size)
                                    { worldSize = size; });
        constexpr auto &worldSizes = magic_enum::enum_values<WorldSize>();

        for (auto &value : worldSizes)
        {
            systemSizeCombobox->addElement(value);
        }
        systemSizeCombobox->setSelectionByText(WorldSize::Medium);

        systemSizeCombobox->setPos(200, y);
        systemSizeCombobox->setWidth(200);
        systemSizeCombobox->setElementFunction([](WorldSize val) -> std::string
                                               { return std::string(magic_enum::enum_name(val)); });
        container->addObject(systemSizeCombobox);

        y += yOffset;
        auto difficultyLabel = std::make_shared<UI::Label>(nullptr);
        difficultyLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        difficultyLabel->setText(_("Difficultiy: "));
        difficultyLabel->setPos(20, y);
        container->addObject(difficultyLabel);

        auto difficultyCombobox = std::make_shared<UI::ComboBox<Difficulty>>();
        difficultyCombobox->setFont("fonts/Audiowide-Regular.ttf", 14);
        constexpr auto &difficulties = magic_enum::enum_values<Difficulty>();
        for (auto &value : difficulties)
        {
            difficultyCombobox->addElement(value);
        }
        difficultyCombobox->setSelectionByText(Difficulty::Normal);

        difficultyCombobox->setPos(200, y);
        difficultyCombobox->setWidth(200);
        difficultyCombobox->setElementFunction([](Difficulty val) -> std::string
                                               { return std::string(magic_enum::enum_name(val)); });
        container->addObject(difficultyCombobox);
        winMgr->addContainer(container.get());
        graphics::Rect bounds = {10, 450, 1280, 400};

        layout->updateLayout(bounds);

        bgTexture = graphics::TextureManager::Instance().loadTexture("images/title_background.png");

        graphics::Rect scrollbounds = {5, 250, renderer->getViewPort().width - 10, renderer->getViewPort().height - 260};

        //auto scrollArea = std::make_shared<UI::ScrollArea>(scrollbounds.width, 190, nullptr);
        //scrollArea->setPos(scrollbounds.x, scrollbounds.y);
        int i = 0;
        // for (auto &race : avaiableRaces)
        // {
        //     auto raceIcon = std::make_shared<UI::ImageButton>(scrollArea.get(), 80, 170, 0, 0, true);
        //     raceIcon->setImage(race->getFaceTexture());
        //     raceIcon->setPos(i * 100, 5);
        //     scrollArea->addObject(raceIcon);
        //     auto raceName = std::make_shared<UI::Label>(scrollArea.get());
        //     raceName->setFont("fonts/Audiowide-Regular.ttf", 14);
        //     raceName->setPos(i * 100, 170);
        //     raceName->setText(race->getName());
        //     if (i == 0)
        //     {
        //         raceName->setColor(utils::color::BLUE);
        //     }
        //     raceIcon->connect("buttonClick", [=]()
        //                       {
        //                           SDL_Color color = utils::color::BLUE;
        //                           SDL_Color white = utils::color::WHITE;
        //                           for (size_t objIdx = 0; objIdx < scrollArea->size(); ++objIdx)
        //                           {
        //                               auto obj = scrollArea->get(objIdx);
        //                               auto label = std::dynamic_pointer_cast<UI::Label>(obj);
        //                               if (label != nullptr)
        //                               {
        //                                   label->setColor(utils::color::WHITE);
        //                               }
        //                           }

        //                           raceName->setColor(color);
        //                           selectedRace = race;
        //                       });

        //     scrollArea->addObject(raceName);

        //     i++;
        // }

        //winMgr->addContainer(scrollArea.get());
    }

    void NewGameScene::render()
    {
        bgTexture->renderResized(renderer, 0, 0,
                                 renderer->getMainCamera()->getWidth(),
                                 renderer->getMainCamera()->getHeight());

        graphics::Texture texture(renderer, renderer->getMainCamera()->getWidth(),
                                  renderer->getMainCamera()->getHeight());

        renderer->setRenderTarget(texture.getSDLTexture());
        renderer->setDrawColor(12, 21, 24, 155);
        graphics::Rect bounds = {5, 250, renderer->getViewPort().width - 10, renderer->getViewPort().height - 260};
        utils::Vector2 lineStart(bounds.x, 440);
        utils::Vector2 lineEnd(bounds.x + bounds.width, 440);

        renderer->fillRect(bounds);
        renderer->setDrawColor(93, 103, 108, 255);
        renderer->drawLine(lineStart, lineEnd);
        renderer->drawRect(bounds);
        renderer->setRenderTarget(nullptr);
        texture.setBlendMode(SDL_BLENDMODE_BLEND);
        texture.render(renderer, 0, 0);

        winMgr->render(renderer);
    }

    void NewGameScene::update()
    {
    }

    void NewGameScene::handleEvents(core::Input *pInput)
    {
        winMgr->handleInput(pInput);
    }

    void NewGameScene::startGame()
    {
        std::cout << "start game" << std::endl;
        world::MapGenerator gen;

        int size = 100 * static_cast<int>(worldSize);

        auto gameMap = gen.generateMap(size, size, seed);
        auto cities = gen.getGeneratedCities();
        auto player = std::make_shared<world::Company>(playerName, 1000000, true);
        auto gameState = std::make_shared<world::GameState>(player, gameMap, cities);

        auto starMapScene = std::make_shared<scenes::WorldScene>(renderer, sceneManager, gameState);
        sceneManager->addScene("world", starMapScene);
        sceneManager->setCurrentScene("world");
    }
} // namespace scenes