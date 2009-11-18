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

	ControlPanel::ControlPanel()
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
		button->setButtonPressed(!button->getButtonPressed());
	}

} // namespace demo
