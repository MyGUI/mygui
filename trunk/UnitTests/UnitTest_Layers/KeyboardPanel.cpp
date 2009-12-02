/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "KeyboardPanel.h"

namespace demo
{

	KeyboardPanel::KeyboardPanel()
	{
		initialiseByAttributes(this);

		mMainWidget->setPosition(0, 0);

		mButtonEngine->eventMouseButtonClick = MyGUI::newDelegate(this, &KeyboardPanel::notifyMouseButtonClick);
		mButtonGun->eventMouseButtonClick = MyGUI::newDelegate(this, &KeyboardPanel::notifyMouseButtonClick);
		mButtonAmmo->eventMouseButtonClick = MyGUI::newDelegate(this, &KeyboardPanel::notifyMouseButtonClick);
		mButtonArmor->eventMouseButtonClick = MyGUI::newDelegate(this, &KeyboardPanel::notifyMouseButtonClick);
	}

	KeyboardPanel::~KeyboardPanel()
	{
	}

	void KeyboardPanel::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		//_sender->castType<MyGUI::Button>()->setButtonPressed(!_sender->castType<MyGUI::Button>()->getButtonPressed());
	}

 } // namespace demo
