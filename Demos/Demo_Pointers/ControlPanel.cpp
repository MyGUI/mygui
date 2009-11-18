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

	ControlPanel::ControlPanel(PointerManager* _manager) :
		mPointerManager(_manager)
	{
		initialiseByAttributes(this);

		const MyGUI::IntSize& size = MyGUI::Gui::getInstance().getViewSize();
		mMainWidget->setPosition(10, size.height - 10 - mMainWidget->getHeight());

		mBusy->eventMouseButtonClick = MyGUI::newDelegate(this, &ControlPanel::notifyMouseButtonClick);
		mRepare->eventMouseButtonClick = MyGUI::newDelegate(this, &ControlPanel::notifyMouseButtonClick);
		mAttack->eventMouseButtonClick = MyGUI::newDelegate(this, &ControlPanel::notifyMouseButtonClick);
	}

	void ControlPanel::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		MyGUI::Button* button = _sender->castType<MyGUI::Button>();
		std::string context = button->getUserString("Context");

		if (button->getButtonPressed())
		{
			button->setButtonPressed(false);
			mPointerManager->addContext(context);
		}
		else
		{
			button->setButtonPressed(true);
			mPointerManager->removeContext(context);
		}

	}

} // namespace demo
