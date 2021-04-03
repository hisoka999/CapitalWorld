/*
 * MessageDialog.cpp
 *
 *  Created on: 01.10.2016
 *      Author: stefan
 */

#include "engine/ui/dialogs/MessageDialog.h"

namespace UI {
namespace dialogs {

    MessageDialog::MessageDialog()
        : UI::Window()
    {
        messageLabel = std::make_shared<UI::Label>(this);
        okButton = std::make_shared<UI::Button>(this);
        cancelButton = std::make_shared<UI::Button>(this);
        setSize(300, 120);
        messageLabel->setPos(10, 10);

        okButton->setPos(10, 50);
        cancelButton->setPos(250, 50);
        addObject(messageLabel);
        okButton->connect("buttonClick", [&]() {
            fireFuncionCall(dialogFinishedCallback(), true);
            setVisible(false);
        });
        cancelButton->connect("buttonClick", [&]() {
            fireFuncionCall(dialogFinishedCallback(), false);
            setVisible(false);
        });
        addObject(okButton);
        addObject(cancelButton);
    }

    void MessageDialog::setMessage(const std::string& message)
    {
        messageLabel->setText(message);
    }
    void MessageDialog::setButtonText(const std::string& okButtonText,
        const std::string& cancelButtonText)
    {
        okButton->setLabel(okButtonText);
        cancelButton->setLabel(cancelButtonText);
    }

    MessageDialog::~MessageDialog()
    {
        // TODO Auto-generated destructor stub
    }

} /* namespace dialogs */
} /* namespace UI */
