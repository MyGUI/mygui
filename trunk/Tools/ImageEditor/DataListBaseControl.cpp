/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataListBaseControl.h"
#include "FactoryManager.h"
#include "CommandManager.h"
#include "DialogManager.h"
#include "MessageBoxManager.h"
#include "DataManager.h"
#include "ActionManager.h"
#include "ActionCreateData.h"
#include "ActionDestroyData.h"
#include "ActionRenameData.h"
#include "ActionChangePositionData.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(DataListBaseControl)

	DataListBaseControl::DataListBaseControl() :
		mNameIndex(0),
		mListBoxControl(nullptr)
	{
	}

	DataListBaseControl::~DataListBaseControl()
	{
	}

	void DataListBaseControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		const VectorControl& childs = getChilds();
		for (VectorControl::const_iterator child = childs.begin(); child != childs.end(); child ++)
		{
			ListBoxDataControl* list = dynamic_cast<ListBoxDataControl*>(*child);
			if (list != nullptr)
			{
				mListBoxControl = list;
				break;
			}
		}

		if (mListBoxControl != nullptr)
		{
			mListBoxControl->setEnableChangePosition(true);
			mListBoxControl->eventChangePosition.connect(this, &DataListBaseControl::notifyChangePosition);
			mListBoxControl->eventChangeName.connect(this, &DataListBaseControl::notifyChangeName);
		}
	}

	bool DataListBaseControl::checkCommand(bool _result)
	{
		if (_result)
			return false;

		if (DialogManager::getInstance().getAnyDialog())
			return false;

		if (MessageBoxManager::getInstance().hasAny())
			return false;

		return true;
	}

	void DataListBaseControl::commandCreateImageData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand(_result))
			return;

		Data* data = DataManager::getInstance().getSelectedDataByType(mParentType);
		if (data != nullptr)
		{
			ActionCreateData* command = new ActionCreateData();
			command->setName(MyGUI::utility::toString("item ", mNameIndex));
			command->setType(mCurrentType);
			command->setPropertyName(mPropertyForName);
			command->setParent(data);

			ActionManager::getInstance().doAction(command);

			mNameIndex ++;
		}

		_result = true;
	}

	void DataListBaseControl::commandDestroyImageData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand(_result))
			return;

		Data* data = DataManager::getInstance().getSelectedDataByType(mCurrentType);
		if (data != nullptr)
		{
			ActionDestroyData* command = new ActionDestroyData();
			command->setData(data);

			ActionManager::getInstance().doAction(command);
		}

		_result = true;
	}

	void DataListBaseControl::commandRenameImageData(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand(_result))
			return;

		if (mListBoxControl != nullptr)
			mListBoxControl->OnRenameData();

		_result = true;
	}

	void DataListBaseControl::setDataInfo(const std::string& _parentType, const std::string& _currentType, const std::string& _property)
	{
		mParentType = _parentType;
		mCurrentType = _currentType;
		mPropertyForName = _property;

		if (mListBoxControl != nullptr)
			mListBoxControl->setDataInfo(mParentType, mPropertyForName);
	}

	void DataListBaseControl::notifyChangePosition(Data* _data1, Data* _data2)
	{
		ActionChangePositionData* command = new ActionChangePositionData();
		command->setData1(_data1);
		command->setData2(_data2);

		ActionManager::getInstance().doAction(command);
	}

	void DataListBaseControl::notifyChangeName(Data* _data, const std::string& _name)
	{
		ActionRenameData* command = new ActionRenameData();
		command->setData(_data);
		command->setName(_name);

		ActionManager::getInstance().doAction(command);
	}

}
