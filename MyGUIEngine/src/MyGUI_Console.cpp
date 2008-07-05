/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#include "MyGUI_Console.h"
#include "MyGUI_CastWidget.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_List.h"
#include "MyGUI_ComboBox.h"
#include "MyGUI_Button.h"

namespace MyGUI
{
	Ogre::String Console::WidgetTypeName = "Console";

	Console::Console(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Window(_coord, _align, _info, _parent, _creator, _name)
	{

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Console_Send") {
				mSendButton = castWidget<Button>(*iter);
				mSendButton->eventMouseButtonClick = newDelegate(this, &Console::notifyMouseButtonClick);
				mSendButton->setCaption("Submit");
			}
			else if ((*iter)->_getInternalString() == "Console_List") {
				mCommandList = castWidget<List>(*iter);
			}
			else if ((*iter)->_getInternalString() == "Console_Command") {
				mCommandInput = castWidget<ComboBox>(*iter);
			}
		}

		MYGUI_ASSERT(null != mCommandList, "Child Widget List not found in skin");
		MYGUI_ASSERT(null != mCommandInput, "Child Widget ComboBox not found in skin");

		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("Current");
		if (iter != param.end()) mStringCurrent = iter->second;
		iter = param.find("Error");
		if (iter != param.end()) mStringError = iter->second;
		iter = param.find("Success");
		if (iter != param.end()) mStringSuccess = iter->second;
		iter = param.find("Unknow");
		if (iter != param.end()) mStringUnknow = iter->second;
		iter = param.find("Format");
		if (iter != param.end()) mStringFormat = iter->second;

		mCommandInput->eventComboAccept = newDelegate(this, &Console::notifyComboAccept);

		this->eventWindowButtonPressed = newDelegate(this, &Console::notifyWindowButtonPressed);
		this->setCaption("Console");
		this->setMinMax(100, 100, 2000, 2000);

		this->registerConsoleDelegate("clear", newDelegate(this, &Console::internalCommand));
	}

	void Console::notifyMouseButtonClick(WidgetPtr _sender)
	{
		notifyComboAccept(mCommandInput);
	}

	void Console::notifyWindowButtonPressed(WidgetPtr _sender, const std::string & _button)
	{
		if (_button == "close") hide();
	}

	void Console::notifyComboAccept(WidgetPtr _sender)
	{
		const Ogre::UTFString & command = _sender->getCaption();
		if (command == "") return;

		Ogre::UTFString key = command;
		Ogre::UTFString value;

		size_t pos = command.find(' ');
		if (pos != Ogre::UTFString::npos) {
			key = command.substr(0, pos);
			value = command.substr(pos + 1);
		}

		MapDelegate::iterator iter = mDelegates.find(key);
		if (iter != mDelegates.end()) {
			iter->second(this, key, value);
		}
		else {
			if (eventConsoleUnknowCommand.IsNull()) {
				addToConsole(mStringUnknow + "'" + key + "'");
			}
			else {
				eventConsoleUnknowCommand(this, key, value);
			}
		}

		_sender->setCaption("");
	}

	void Console::addToConsole(const Ogre::UTFString & _line)
	{
		mCommandList->addItem(_line);
		mCommandList->beginToEnd();
	}

	void Console::clearConsole()
	{
		mCommandList->deleteAllItems();
	}

	void Console::registerConsoleDelegate(const Ogre::UTFString & _command, delegates::IDelegate3<WidgetPtr, const Ogre::UTFString &, const Ogre::UTFString &> * _delegate, const Ogre::UTFString & _format)
	{
		mCommandInput->addItem(_command);
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end()) {
			MYGUI_LOG(Warning, utility::toString("console - command '", _command, "' already exist"));
		}
		mDelegates[_command] = _delegate;
		if ( ! _format.empty()) mFormates[_command] = _format;
	}

	void Console::internalCommand(MyGUI::WidgetPtr _sender, const Ogre::UTFString & _key, const Ogre::UTFString & _value)
	{
		if (_key == "clear") {
			clearConsole();
		}
	}

	void Console::printCommandFormat(const Ogre::UTFString & _command)
	{
		MapFormat::iterator iter = mFormates.find(_command);
		if (iter != mFormates.end()) addToConsole(getConsoleStringFormat() + iter->second);
		else addToConsole(getConsoleStringFormat() + "not found");
	}

	const Ogre::UTFString & Console::getCommandFormat(const Ogre::UTFString & _command)
	{
		MapFormat::iterator iter = mFormates.find(_command);
		if (iter != mFormates.end()) return iter->second;
		else {
			static Ogre::UTFString empty;
			return empty;
		}
	}

	void Console::showConsole()
	{
		show();
		InputManager::getInstance().setKeyFocusWidget(mCommandInput);
	}

	void Console::hideConsole()
	{
		hide();
		InputManager::getInstance().resetKeyFocusWidget(mCommandInput);
		InputManager::getInstance().resetKeyFocusWidget(mCommandList);
		InputManager::getInstance().resetKeyFocusWidget(mSendButton);
		InputManager::getInstance().resetKeyFocusWidget(this);
	}

} // namespace MyGUI
