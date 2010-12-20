/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#include "Precompiled.h"
#include "MainPanel.h"

namespace demo
{

	MainPanel::MainPanel() :
		BaseLayout("MainPanel.layout")
	{
		//mMainWidget->setPosition(10, 10);

		assignWidget(mButtonNew, "New");
		assignWidget(mButtonLoad, "Load");
		assignWidget(mButtonQuit, "Quit");
		assignWidget(mComboCreate, "CreateObject");

		mButtonNew->eventMouseButtonClick += MyGUI::newDelegate(this, &MainPanel::notifyMouseButtonClick);
		mButtonLoad->eventMouseButtonClick += MyGUI::newDelegate(this, &MainPanel::notifyMouseButtonClick);
		mButtonQuit->eventMouseButtonClick += MyGUI::newDelegate(this, &MainPanel::notifyMouseButtonClick);
		mComboCreate->eventComboAccept += MyGUI::newDelegate(this, &MainPanel::notifyComboAccept);
	}

	void MainPanel::addObject(const MyGUI::UString& _name)
	{
		mComboCreate->addItem(_name);
	}

	void MainPanel::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		if (_sender == mButtonNew)
			eventAction(EventNew, 0);
		else if (_sender == mButtonLoad)
			eventAction(EventLoad, 0);
		else if (_sender == mButtonQuit)
			eventAction(EventQuit, 0);
	}

	void MainPanel::notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
	{
		eventAction(EventCreate, _index);

		mComboCreate->setIndexSelected(MyGUI::ITEM_NONE);
		mComboCreate->setCaption("Select Type");
	}

} // namespace demo
