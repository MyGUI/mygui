/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#include "Precompiled.h"
#include "DataListUI.h"
#include "ActionManager.h"
#include "ActionCreateData.h"
#include "ActionDestroyData.h"
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

		tools::ActionManager::getInstance().eventChanges.connect(this, &DataListUI::updateActions);
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
			tools::ActionManager::getInstance().undoAction();
		}
		else if (event == "Redo")
		{
			tools::ActionManager::getInstance().redoAction();
		}
		else if (event == "Add")
		{
			tools::ActionCreateData* command = new tools::ActionCreateData();
			command->setName(MyGUI::utility::toString("item ", mIndex));

			tools::ActionManager::getInstance().doAction(command);

			mIndex ++;
		}
		else if (event == "Remove")
		{
			if (mListBox->getItemCount() != 0)
			{
				tools::ActionDestroyData* command = new tools::ActionDestroyData();
				tools::Data* data = *mListBox->getItemDataAt<tools::Data*>(mListBox->getItemCount() - 1);
				command->setData(data);
				tools::ActionManager::getInstance().doAction(command);
			}
		}
		else if (event == "Save")
		{
			tools::ActionManager::getInstance().resetChanges();
		}
	}

	void DataListUI::updateActions()
	{
		mTextBox->setCaption(tools::ActionManager::getInstance().getChanges() ? "*" : "");
	}

	void DataListUI::updateListData()
	{
		mListBox->removeAllItems();

		const tools::Data::VectorData& childs = tools::DataManager::getInstance().getRoot()->getChilds();
		for (tools::Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child ++)
		{
			tools::Data* data = *(child);
			mListBox->addItem((*child)->getPropertyValue("Name"), data);
		}
	}
}
