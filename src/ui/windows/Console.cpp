#include "Console.h"
#include <engine/ui/Label.h>
#include <engine/utils/string.h>

namespace UI
{
    Console::Console(const std::shared_ptr<world::GameState> &gameState) : UI::Window(0, 50, 250, 200), gameState(gameState)
    {
        initUI();
    }

    Console::~Console()
    {
    }

    void Console::initUI()
    {
        scrollArea = std::make_shared<UI::ScrollArea>(220, 130, this);
        addObject(scrollArea);

        textItem = std::make_shared<UI::TextItem>(this, 190, 20);
        textItem->setPos(5, 140);
        textItem->connect("inputSubmit", [&](std::string value)
                          {
                            textItem->setText("");
                            auto result = executeCommand(value);

                            for (auto& line : result)
                            {
                                auto label = std::make_shared<UI::Label>(scrollArea.get());
                                
                                label->setText(line);

                                label->setPos(0,scrollArea->size()*15);
                                scrollArea->addObject(label);
                            } });
        addObject(textItem);
    }

    std::vector<std::string> Console::executeCommand(std::string value)
    {
        auto values = utils::split(value, " ");

        if (values[0] == "cash")
        {
            // gameState->getPlayer()->incCash(1000000);
            for (auto &company : gameState->getCompanies())
            {
                company->incCash(1000000);
            }
            return {"added 1 million to player"};
        }
        else if (values[0] == "lr")
        {
            std::vector<std::string> result;
            for (auto &company : gameState->getCompanies())
            {
                result.push_back("company: " + company->getName());
                for (auto &research : company->getResearchQueue())
                {
                    result.push_back(" - " + research->getName());
                }
            }
            return result;
        }

        else
        {
            return {"command \"" + values[0] + "\" not found."};
        }
    }
}