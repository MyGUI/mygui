/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "PanelDirector.h"

namespace demo
{

	PanelDirector::PanelDirector() :
		BasePanelViewItem("PanelDirector.layout"),
		mCheckShowStatic(nullptr),
		mCheckShowDynamic(nullptr),
		mComboCount(nullptr)
	{
	}

	void PanelDirector::initialise()
	{
		mPanelCell->setCaption("Director panel");

		assignWidget(mCheckShowStatic, "check_Static");
		assignWidget(mCheckShowDynamic, "check_Dynamic");
		assignWidget(mComboCount, "combo_Count");

		mCheckShowStatic->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelDirector::notifyMouseButtonClick);
		mCheckShowDynamic->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelDirector::notifyMouseButtonClick);
		mComboCount->eventComboAccept += MyGUI::newDelegate(this, &PanelDirector::notifyComboAccept);
		mComboCount->setIndexSelected(4);
	}

	void PanelDirector::shutdown()
	{
	}

	void PanelDirector::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		if (_sender == mCheckShowStatic)
		{
			mCheckShowStatic->setStateSelected( ! mCheckShowStatic->getStateSelected());
			eventChangePanels(EVENT_SHOW_STATIC, (size_t)mCheckShowStatic->getStateSelected());
		}
		else if (_sender == mCheckShowDynamic)
		{
			mCheckShowDynamic->setStateSelected( ! mCheckShowDynamic->getStateSelected());
			eventChangePanels(EVENT_SHOW_DYNAMIC, (size_t)mCheckShowDynamic->getStateSelected());
		}
	}

	void PanelDirector::notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
	{
		eventChangePanels(EVENT_COUNT_DYNAMIC, MyGUI::utility::parseInt(_sender->getOnlyText()));
	}

} // namespace demo
