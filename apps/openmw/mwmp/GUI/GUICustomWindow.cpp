#include "GUICustomWindow.hpp"

#include <MyGUI_Widget.h>
#include <MyGUI_EditBox.h>

#include "../Networking.hpp"
#include "../Main.hpp"
#include "../LocalPlayer.hpp"

#include <components/openmw-mp/TimedLog.hpp>

namespace mwmp
{
    const std::string GUICustomWindow::BUTTON_PRESSED = "ButtonPressed";
    const std::string GUICustomWindow::MOUSE_CLICK = "MouseClick";
    const std::string GUICustomWindow::FIELD = "Field";

    GUICustomWindow::GUICustomWindow(const std::string& layout): WindowBase(layout)
    {
        for (MyGUI::Widget* widget : mListWindowRoot)
        {
            attachEventHandlers(widget);
            findFields(widget);
        }
    }

    void GUICustomWindow::log(std::string event, std::string name, std::string data) {
        LOG_MESSAGE_SIMPLE(
            TimedLog::LOG_VERBOSE, "Custom GUI event: %s, tag: %s, data: %s",
            event.c_str(), name.c_str(), data.c_str()
        );
    }

    void GUICustomWindow::send(std::string tag, std::string data) {
        LocalPlayer* localPlayer = Main::get().getLocalPlayer();
        Networking* networking = Main::get().getNetworking();

        localPlayer->guiEvent.tag = tag;
        localPlayer->guiEvent.data = data;

        collectFields();

        networking->getPlayerPacket(ID_GUI_EVENT)->setPlayer(localPlayer);
        networking->getPlayerPacket(ID_GUI_EVENT)->Send();
    }

    void GUICustomWindow::attachEventHandlers(MyGUI::Widget* widget)
    {
        int children = widget->getChildCount();
        for (int i = 0; i < children; i++) {
            auto child = widget->getChildAt(i);
            attachEventHandlers(child);
        }
        if (!widget->getUserString(BUTTON_PRESSED).empty()) {
            widget->eventKeyButtonPressed = newDelegate(this, &GUICustomWindow::buttonPressed);
        }
        if (!widget->getUserString(MOUSE_CLICK).empty()) {
            widget->eventMouseButtonClick = newDelegate(this, &GUICustomWindow::mouseClick);
        }
    }

    void GUICustomWindow::buttonPressed(MyGUI::Widget* _sender, MyGUI::KeyCode key, MyGUI::Char _char) {
        std::string tag = _sender->getUserString(BUTTON_PRESSED);
        std::string data = std::to_string(_char);
        log(BUTTON_PRESSED, tag, data);
        send(tag, data);
    }

    void GUICustomWindow::mouseClick(MyGUI::Widget* _sender) {
        std::string tag = _sender->getUserString(MOUSE_CLICK);
        std::string data = "";
        log(MOUSE_CLICK, tag, data);
        send(tag, data);
    }

    void GUICustomWindow::findFields(MyGUI::Widget* widget) {
        int children = widget->getChildCount();
        for (int i = 0; i < children; i++) {
            auto child = widget->getChildAt(i);
            findFields(child);
        }
        std::string key = widget->getUserString(FIELD);
        if (!key.empty()) {
            fieldWidgets[key] = widget;
        }
    }

    void GUICustomWindow::collectFields() {
        LocalPlayer* localPlayer = Main::get().getLocalPlayer();
        localPlayer->guiEvent.fields.clear();

        for (auto widgetIterator: fieldWidgets) {
            std::string key = widgetIterator.first;
            MyGUI::Widget* widget = widgetIterator.second;
            std::string value = "";
            MyGUI::EditBox* editBox = dynamic_cast<MyGUI::EditBox*>(widget);
            if (editBox != NULL)
                value = editBox->getCaption();
            localPlayer->guiEvent.fields.push_back(make_pair(key, value));
        }
    }
}
