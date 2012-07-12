/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "Control.h"
#include "FactoryManager.h"

namespace tools
{
	Control::Control()
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

	void Control::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		if (_parent != nullptr)
		{
			initialise(_layoutName, _place);
			_parent->mChilds.push_back(this);
		}
		else
		{
			initialise(_layoutName, nullptr);
		}

		for (size_t index = 0; index < getRoot()->getChildCount(); index ++)
			CreateChilds(this, getRoot()->getChildAt(index));
	}

	void Control::CreateChilds(Control* _parent, MyGUI::Widget* _widget)
	{
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
}
