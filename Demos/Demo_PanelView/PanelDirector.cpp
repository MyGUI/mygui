/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "PanelDirector.h"

namespace demo
{

	PanelDirector::PanelDirector() : BasePanelViewItem("PanelDirector.layout")
	{
	}

	void PanelDirector::initialise()
	{
		mPanelCell->setCaption("Director panel");

		assignWidget(mCheckShowStatic, "check_Static");
		assignWidget(mCheckShowDynamic, "check_Dynamic");
		assignWidget(mComboCount, "combo_Count");

		mCheckShowStatic->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelDirector::notifyMouseButtonClick);
		mCheckShowDynamic->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelDirector::notifyMouseButtonClick);
		mComboCount->eventComboAccept = MyGUI::newDelegate(this, &PanelDirector::notifyComboAccept);
		mComboCount->setIndexSelected(4);
	}

	void PanelDirector::shutdown()
	{
	}

	void PanelDirector::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		if (_sender == mCheckShowStatic) {
			mCheckShowStatic->setButtonPressed( ! mCheckShowStatic->getButtonPressed());
			eventChangePanels(EVENT_SHOW_STATIC, (size_t)mCheckShowStatic->getButtonPressed());
		}
		else if (_sender == mCheckShowDynamic) {
			mCheckShowDynamic->setButtonPressed( ! mCheckShowDynamic->getButtonPressed());
			eventChangePanels(EVENT_SHOW_DYNAMIC, (size_t)mCheckShowDynamic->getButtonPressed());
		}
	}

	void PanelDirector::notifyComboAccept(MyGUI::ComboBoxPtr _sender, size_t _index)
	{
		eventChangePanels(EVENT_COUNT_DYNAMIC, MyGUI::utility::parseInt(_sender->getCaption()));
	}

} // namespace demo
