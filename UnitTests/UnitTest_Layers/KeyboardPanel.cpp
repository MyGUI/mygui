/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "KeyboardPanel.h"
#include "CommandManager.h"

namespace demo
{

	KeyboardPanel::KeyboardPanel()
	{
		initialiseByAttributes(this);

		mMainWidget->setPosition(0, 0);

		mButtonEngine->eventMouseButtonClick += MyGUI::newDelegate(this, &KeyboardPanel::notifyMouseButtonClick);
		mButtonGun->eventMouseButtonClick += MyGUI::newDelegate(this, &KeyboardPanel::notifyMouseButtonClick);
		mButtonAmmo->eventMouseButtonClick += MyGUI::newDelegate(this, &KeyboardPanel::notifyMouseButtonClick);
		mButtonArmor->eventMouseButtonClick += MyGUI::newDelegate(this, &KeyboardPanel::notifyMouseButtonClick);

		CommandManager::getInstance().executeCommand("KeyboardClick", mMainWidget->getUserString("Command"));
	}

	void KeyboardPanel::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		MyGUI::Button* button = _sender->castType<MyGUI::Button>();
		bool selected = button->getStateSelected();
		if (selected)
		{
			button->setStateSelected(false);
			CommandManager::getInstance().executeCommand("KeyboardClick", mMainWidget->getUserString("Command"));
		}
		else
		{
			mButtonEngine->setStateSelected(false);
			mButtonGun->setStateSelected(false);
			mButtonAmmo->setStateSelected(false);
			mButtonArmor->setStateSelected(false);
			button->setStateSelected(true);
			CommandManager::getInstance().executeCommand("KeyboardClick", button->getUserString("Command"));
		}
	}

} // namespace demo
