/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "Control.h"
#include "FactoryManager.h"
#include "CommandManager.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(Control);

	Control::Control() :
		mParent(nullptr)
	{
	}

	Control::~Control()
	{
		for (VectorControl::iterator child = mChilds.begin(); child != mChilds.end(); child ++)
			delete *child;
		mChilds.clear();
	}

	MyGUI::Widget* Control::getRoot()
	{
		return mMainWidget;
	}

	void Control::Initialise(const std::string& _layoutName)
	{
		Initialise(nullptr, nullptr, _layoutName);
	}

	void Control::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		mParent = _parent;
		if (_parent != nullptr)
		{
			initialise(_layoutName, _place);
			_parent->mChilds.push_back(this);
		}
		else
		{
			initialise(_layoutName, nullptr);
		}

		CheckTabControl(mMainWidget);

		for (size_t index = 0; index < getRoot()->getChildCount(); index ++)
			CreateChilds(this, getRoot()->getChildAt(index));
	}

	void Control::CreateChilds(Control* _parent, MyGUI::Widget* _widget)
	{
		std::string command = _widget->getUserString("CommandClick");
		if (!command.empty())
			_widget->eventMouseButtonClick += MyGUI::newDelegate(this, &Control::notifyMouseButtonClick);

		CheckTabControl(_widget);

		std::string controlType = _widget->getUserString("ControlType");
		if (!controlType.empty())
		{
			std::string controlLayout = _widget->getUserString("ControlLayout");

			components::IFactoryItem* item = components::FactoryManager::GetInstance().CreateItem(controlType);
			if (item != nullptr)
			{
				Control* control = dynamic_cast<Control*>(item);
				if (control != nullptr)
				{
					control->Initialise(_parent, _widget, controlLayout);
					return;
				}
				else
				{
					delete item;
				}
			}
		}

		for (size_t index = 0; index < _widget->getChildCount(); index ++)
			CreateChilds(_parent, _widget->getChildAt(index));
	}

	void Control::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		OnCommand(_sender->getUserString("CommandClick"));
	}

	void Control::OnCommand(const std::string& _commandName, MyGUI::Any _data)
	{
		if (mParent != nullptr)
		{
			mParent->OnCommand(_commandName, _data);
		}
		else
		{
			//CommandManager::getInstance().setCommandData
			CommandManager::getInstance().executeCommand(_commandName);
		}
	}

	const Control::VectorControl& Control::getChilds() const
	{
		return mChilds;
	}

	void Control::notifyTabChangeSelect(MyGUI::TabControl* _sender, size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
			OnCommand(_sender->getItemAt(_index)->getUserString("CommandActivate"));
	}

	void Control::CheckTabControl(MyGUI::Widget* _widget)
	{
		MyGUI::TabControl* tab = _widget->castType<MyGUI::TabControl>(false);
		if (tab != nullptr)
		{
			for (size_t index = 0; index < tab->getItemCount(); index ++)
			{
				if (tab->getItemAt(index)->getUserString("CommandActivate") != "")
				{
					tab->eventTabChangeSelect += MyGUI::newDelegate(this, &Control::notifyTabChangeSelect);
					break;
				}
			}
		}
	}
}
