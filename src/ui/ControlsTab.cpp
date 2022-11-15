#include "ControlsTab.h"
#include <engine/ui/layout/GridLayout.h>
#include <engine/ui/Label.h>
#include <engine/ui/KeyCodeInputItem.h>
#include "translate.h"

namespace UI
{
    ControlsTab::ControlsTab(Object *parent, const std::string &title, core::Input *input, std::shared_ptr<utils::IniBase> &settings)
        : UI::Tab(parent, title), input(input), settings(settings)
    {
        auto layout = std::make_shared<UI::layout::GridLayout>(this, 3);
        setLayout(layout);
        auto keyMap = input->getKeyMap();
        std::vector<std::string> keyList;
        for (auto &[key, value] : keyMap)
        {
            if (std::find(keyList.begin(), keyList.end(), key) == keyList.end())
            {
                keyList.emplace_back(key);
            }
        }
        for (auto &key : keyList)
        {
            auto label = std::make_shared<UI::Label>(_(key), this);
            addObject(label);
            auto values = keyMap.equal_range(key);

            auto it = values.first;
            SDL_Keycode keyCode = it->second;
            if (settings->getValue("keys", key + "_1").length() > 0)
                keyCode = settings->getValueI("keys", key + "_1");

            auto keyInput1 = std::make_shared<UI::KeyCodeInputItem>(this, 180, 30);
            keyInput1->setValue(keyCode);
            keyInput1->connect("keyCodeChanged", [this, key](SDL_Keycode keycode)
                               { this->settings->setAttrI("keys", key + "_1", keycode); });

            it++;

            addObject(keyInput1);
            auto keyInput2 = std::make_shared<UI::KeyCodeInputItem>(this, 180, 30);

            if (settings->getValue("keys", key + "_2").length() > 0)
                keyInput2->setValue(settings->getValueI("keys", key + "_2"));
            else if (it != values.second)
                keyInput2->setValue(it->second);
            keyInput2->connect("keyCodeChanged", [this, key](SDL_Keycode keycode)
                               { this->settings->setAttrI("keys", key + "_2", keycode); });
            addObject(keyInput2);
        }

        graphics::Rect bounds = {10, 10, float(this->getWidth()), float(this->getHeight())};

        layout->updateLayout(bounds);
    }

    ControlsTab::~ControlsTab()
    {
    }
}