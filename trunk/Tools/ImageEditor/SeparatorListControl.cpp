/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "SeparatorListControl.h"
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

	FACTORY_ITEM_ATTRIBUTE(SeparatorListControl)

	SeparatorListControl::SeparatorListControl() :
		mListBoxControl(nullptr)
	{
	}

	SeparatorListControl::~SeparatorListControl()
	{
	}

	void SeparatorListControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		mListBoxControl = findControl<ListBoxDataControl>();

		if (mListBoxControl != nullptr)
		{
			mListBoxControl->setEnableChangePosition(false);
			mListBoxControl->setReplaceColourName("ColourDisabled");
		}

		setDataInfo("Skin", "Separator", "Name", "Visible");
	}

	void SeparatorListControl::setDataInfo(const std::string& _parentType, const std::string& _currentType, const std::string& _propertyName, const std::string& _propertyUnique)
	{
		mParentType = _parentType;
		mPropertyForName = _propertyName;
		mPropertyForUnique = _propertyUnique;

		if (mListBoxControl != nullptr)
		{
			mListBoxControl->setDataInfo(mParentType, _currentType, mPropertyForName);
			mListBoxControl->addPropertyNameEnabled(mPropertyForUnique);
		}
	}

}
