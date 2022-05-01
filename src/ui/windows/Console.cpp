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

                              auto label = std::make_shared<UI::Label>(scrollArea.get());

                              
                              label->setText(result);

                              label->setPos(0,scrollArea->size()*15);
                              scrollArea->addObject(label); });
        addObject(textItem);
    }

    std::string Console::executeCommand(std::string value)
    {
        auto values = utils::split(value, " ");

        if (values[0] == "cash")
        {
            gameState->getPlayer()->incCash(1000000);
            return "added 1 million to player";
        }
        else
        {
            return "command \"" + values[0] + "\" not found.";
        }
    }
}