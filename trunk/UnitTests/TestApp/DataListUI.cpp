/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#include "Precompiled.h"
#include "DataListUI.h"
#include "CommandManager.h"
#include "CommandCreateData.h"
#include "CommandDestroyData.h"
#include "DataManager.h"
#include "Data.h"

namespace demo
{
	DataListUI::DataListUI() :
		mIndex(0)
	{
		initialiseByAttributes(this);

		mTextBox->setCaption("");

		bindEvents(mMainWidget);

		tools::CommandManager::getInstance().eventChangeCommands.connect(this, &DataListUI::updateCommands);
		tools::DataManager::getInstance().eventChangeData.connect(this, &DataListUI::updateListData);
	}

	void DataListUI::bindEvents(MyGUI::Widget* _widget)
	{
		MyGUI::Button* button = _widget->castType<MyGUI::Button>(false);
		if (button != nullptr)
		{
			std::string event = button->getUserString("Event");
			if (event != "")
				button->eventMouseButtonClick += MyGUI::newDelegate(this, &DataListUI::notifyMouseButtonClick);
		}

		for (size_t index = 0; index < _widget->getChildCount(); index ++)
			bindEvents(_widget->getChildAt(index));
	}

	void DataListUI::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		std::string event = _sender->getUserString("Event");

		if (event == "Undo")
		{
			tools::CommandManager::getInstance().undoCommand();
		}
		else if (event == "Redo")
		{
			tools::CommandManager::getInstance().redoCommand();
		}
		else if (event == "Add")
		{
			tools::CommandCreateData* data = new tools::CommandCreateData();
			data->setName(MyGUI::utility::toString("item ", mIndex));

			tools::CommandManager::getInstance().doCommand(data);

			mIndex ++;
		}
		else if (event == "Remove")
		{
			tools::CommandDestroyData* data = new tools::CommandDestroyData();
			tools::CommandManager::getInstance().doCommand(data);
		}
		else if (event == "Save")
		{
			tools::CommandManager::getInstance().setCurrentCommandAsSave();
		}
	}

	void DataListUI::updateCommands()
	{
		mTextBox->setCaption(tools::CommandManager::getInstance().getCurrentCommandAsSave() ? "" : "*");
	}

	void DataListUI::updateListData()
	{
		mListBox->removeAllItems();

		const tools::Data::VectorData& childs = tools::DataManager::getInstance().getRoot()->getChilds();
		for (tools::Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child ++)
		{
			mListBox->addItem((*child)->getPropertyValue("Name"), *(child));
		}
	}
}
