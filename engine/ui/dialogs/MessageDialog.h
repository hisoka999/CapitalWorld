/*
 * MessageDialog.h
 *
 *  Created on: 01.10.2016
 *      Author: stefan
 */

#ifndef UI_DIALOGS_MESSAGEDIALOG_H_
#define UI_DIALOGS_MESSAGEDIALOG_H_

#include <engine/ui/Window.h>
#include <engine/ui/Label.h>
#include <engine/ui/Button.h>

namespace UI {
namespace dialogs {

class MessageDialog: public Window {
public:
    MessageDialog();
    virtual ~MessageDialog();
    void setMessage(const std::string& message);
    void setButtonText(const std::string& okButtonText,
            const std::string& cancelButtonText);

    static std::string dialogFinishedCallback() {
        return "dialogFinished";
    }
private:
    UI::Button okButton;
    UI::Button cancelButton;
    UI::Label messageLabel;
};

} /* namespace dialogs */
} /* namespace UI */

#endif /* UI_DIALOGS_MESSAGEDIALOG_H_ */
