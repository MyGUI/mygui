/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/
#include "precompiled.h"
#include "ControlPanel.h"

namespace demo
{

	ControlPanel::ControlPanel(PointerContextManager* _manager) :
		mPointerContextManager(_manager)
	{
		initialiseByAttributes(this);

		const MyGUI::IntSize& size = MyGUI::Gui::getInstance().getViewSize();
		mMainWidget->setPosition(10, size.height - 10 - mMainWidget->getHeight());

		mBusy->eventMouseButtonClick = MyGUI::newDelegate(this, &ControlPanel::notifyMouseButtonClick);
		mRepair->eventMouseButtonClick = MyGUI::newDelegate(this, &ControlPanel::notifyMouseButtonClick);
		mAttack->eventMouseButtonClick = MyGUI::newDelegate(this, &ControlPanel::notifyMouseButtonClick);
	}

	void ControlPanel::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		MyGUI::Button* button = _sender->castType<MyGUI::Button>();
		std::string context = button->getUserString("Context");

		bool pressed = button->getButtonPressed();
		button->setButtonPressed(!pressed);
		if (pressed)
			mPointerContextManager->removeContext(context);
		else
			mPointerContextManager->addContext(context);

		// сбрасываем радио
		if (button == mRepair)
		{
			if (mAttack->getButtonPressed())
			{
				mAttack->setButtonPressed(false);
				mPointerContextManager->removeContext(mAttack->getUserString("Context"));
			}
		}
		else if (button == mAttack)
		{
			if (mRepair->getButtonPressed())
			{
				mRepair->setButtonPressed(false);
				mPointerContextManager->removeContext(mRepair->getUserString("Context"));
			}
		}

	}

	void ControlPanel::injectKeyPress(MyGUI::KeyCode _key)
	{
		if (_key == MyGUI::KeyCode::F1)
			notifyMouseButtonClick(mBusy);
		else if (_key == MyGUI::KeyCode::F2)
			notifyMouseButtonClick(mRepair);
		else if (_key == MyGUI::KeyCode::F3)
			notifyMouseButtonClick(mAttack);
	}

} // namespace demo
