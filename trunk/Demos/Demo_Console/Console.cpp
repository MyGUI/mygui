/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "Console.h"

Console::Console() :
	BaseLayout("Console.layout")
{
}

void Console::initialise()
{
	loadLayout();

	assignWidget(mListHistory, "list_History");
	assignWidget(mComboCommand, "combo_Command");
	assignWidget(mButtonSubmit, "button_Submit");

	if (mMainWidget->getWidgetType() == "Window") {
		static_cast<MyGUI::WindowPtr>(mMainWidget)->eventWindowButtonPressed = newDelegate(this, &Console::notifyWindowButtonPressed);
	}

	mStringCurrent = mMainWidget->getUserString("Current");
	mStringError = mMainWidget->getUserString("Error");
	mStringSuccess = mMainWidget->getUserString("Success");
	mStringUnknow = mMainWidget->getUserString("Unknow");
	mStringFormat = mMainWidget->getUserString("Format");

	mComboCommand->eventComboAccept = newDelegate(this, &Console::notifyComboAccept);
	mButtonSubmit->eventMouseButtonClick = newDelegate(this, &Console::notifyMouseButtonClick);
}

void Console::show()
{
	mMainWidget->show();
}

void Console::hide()
{
	mMainWidget->hide();
}

void Console::notifyWindowButtonPressed(MyGUI::WidgetPtr _sender, const std::string & _button)
{
	if (_button == "close") {
		mMainWidget->hide();
	}
}

void Console::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
{
	notifyComboAccept(mComboCommand);
}

void Console::notifyComboAccept(MyGUI::WidgetPtr _sender)
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
		iter->second(key, value);
	}
	else {
		if (eventConsoleUnknowCommand.empty()) {
			addToConsole(mStringUnknow + "'" + key + "'");
		}
		else {
			eventConsoleUnknowCommand(key, value);
		}
	}

	_sender->setCaption("");
}

void Console::addToConsole(const Ogre::UTFString & _line)
{
	mListHistory->addItem(_line);
	mListHistory->beginToEnd();
}

void Console::clearConsole()
{
	mListHistory->deleteAllItems();
}

void Console::registerConsoleDelegate(const Ogre::UTFString & _command, DelegatePtr _delegate, const Ogre::UTFString & _format)
{
	mComboCommand->addItem(_command);
	MapDelegate::iterator iter = mDelegates.find(_command);
	if (iter != mDelegates.end()) {
		MYGUI_LOG(Warning, "console - command '" << _command << "' already exist");
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
