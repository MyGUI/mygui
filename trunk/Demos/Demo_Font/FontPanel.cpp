/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "FontPanel.h"

FontPanel::FontPanel() :
	BaseLayout("FontPanel.layout")
{
}

void FontPanel::initialise()
{
	loadLayout();

	/*assignWidget(mListHistory, "list_History");
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

	mAutocomleted = false;

	mComboCommand->eventComboAccept = newDelegate(this, &Console::notifyComboAccept);
	mComboCommand->eventEditTextChange = newDelegate(this, &Console::notifyCommandPrint);
	mComboCommand->eventKeyButtonPressed = newDelegate(this, &Console::notifyButtonPressed);
	mButtonSubmit->eventMouseButtonClick = newDelegate(this, &Console::notifyMouseButtonClick);*/
}

void FontPanel::show()
{
	mMainWidget->show();
}

void FontPanel::hide()
{
	mMainWidget->hide();
}
