/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "KeyboardPanel.h"
#include "CommandManager.h"

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

		CommandManager::getInstance().execiteCommand("KeyboardClick", std::string(mMainWidget->getUserString("Command")));
	}

	KeyboardPanel::~KeyboardPanel()
	{
	}

	void KeyboardPanel::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		MyGUI::Button* button = _sender->castType<MyGUI::Button>();
		bool selected = button->getButtonPressed();
		if (selected)
		{
			button->setButtonPressed(false);
			CommandManager::getInstance().execiteCommand("KeyboardClick", std::string(mMainWidget->getUserString("Command")));
		}
		else
		{
			mButtonEngine->setButtonPressed(false);
			mButtonGun->setButtonPressed(false);
			mButtonAmmo->setButtonPressed(false);
			mButtonArmor->setButtonPressed(false);
			button->setButtonPressed(true);
			CommandManager::getInstance().execiteCommand("KeyboardClick", std::string(button->getUserString("Command")));
		}
	}

 } // namespace demo
