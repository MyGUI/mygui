/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "RegionListControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"
#include "DialogManager.h"
#include "MessageBoxManager.h"
#include "DataManager.h"
#include "ActionManager.h"
#include "ActionCreateData.h"
#include "ActionCloneData.h"
#include "ActionDestroyData.h"
#include "ActionRenameData.h"
#include "ActionChangePositionData.h"
#include "PropertyUtility.h"
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(RegionListControl)

	RegionListControl::RegionListControl() :
		mListBoxControl(nullptr)
	{
	}

	RegionListControl::~RegionListControl()
	{
	}

	void RegionListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		mListBoxControl = findControl<ListBoxDataControl>();

		if (mListBoxControl != nullptr)
		{
			mListBoxControl->setEnableChangePosition(false);
			mListBoxControl->setReplaceColourName("ColourDisabled");

			mListBoxControl->addPropertyNameEnabled("Visible");
			mListBoxControl->addPropertyNameEnabled("Enable");

			mListBoxControl->setDataInfo("Skin", "Region", "Name");
		}
	}

}
