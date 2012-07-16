/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "PropertyPanelControl.h"
#include "FactoryManager.h"
#include "ScopeManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "DataTypeManager.h"
#include "PropertyInt2Control.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(PropertyPanelControl)

	PropertyPanelControl::PropertyPanelControl() :
		mParentType(nullptr),
		mCurrentData(nullptr)
	{
	}

	PropertyPanelControl::~PropertyPanelControl()
	{
	}

	void PropertyPanelControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		ScopeManager::getInstance().eventChangeScope.connect(this, &PropertyPanelControl::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());
	}

	void PropertyPanelControl::notifyChangeScope(const std::string& _scope)
	{
		if (mParentType != nullptr)
		{
			DataSelectorManager::getInstance().getEvent(mParentType->getName())->disconnect(this);
			mParentType = nullptr;
		}

		mParentType = DataTypeManager::getInstance().getParentType(_scope);

		if (mParentType != nullptr)
		{
			DataSelectorManager::getInstance().getEvent(mParentType->getName())->connect(this, &PropertyPanelControl::notifyChangeDataSelector);

			Data* parentData = DataManager::getInstance().getSelectedDataByType(mParentType->getName());
			notifyChangeDataSelector(parentData, false);
		}
	}

	void PropertyPanelControl::notifyChangeDataSelector(Data* _data, bool _changeOnlySelection)
	{
		/*std::string text;

		if (mParentType != nullptr)
			text += MyGUI::utility::toString(mParentType->getName(), "\n\n");

		if (_data != nullptr)
		{
			Data* selected = _data->getChildSelected();
			if (selected != nullptr)
			{
				const Data::MapString& properties = selected->getProperties();
				for (Data::MapString::const_iterator property = properties.begin(); property != properties.end(); property ++)
				{
					text += MyGUI::utility::toString((*property).first, " = ", (*property).second, "\n");
				}
			}
		}

		mText->setCaption(text);*/
		HideControls();

		if (mCurrentData != nullptr)
		{
			mCurrentData = nullptr;
		}

		if (_data != nullptr)
			mCurrentData = _data->getChildSelected();

		if (mCurrentData != nullptr)
		{
			const Data::MapString& properties = mCurrentData->getProperties();
			for (Data::MapString::const_iterator property = properties.begin(); property != properties.end(); property ++)
			{
				InitialiseProperty((*property).second);
			}
		}
	}

	void PropertyPanelControl::HideControls()
	{
		for (VectorPairControl::iterator control = mPropertyControls.begin(); control != mPropertyControls.end(); control ++)
		{
			(*control).second->setProperty(nullptr);
			(*control).second->getRoot()->setVisible(false);
		}
	}

	void PropertyPanelControl::InitialiseProperty(Property* _property)
	{
		std::string type = _property->getType()->getType();

		for (VectorPairControl::iterator control = mPropertyControls.begin(); control != mPropertyControls.end(); control ++)
		{
			if ((*control).first == type && !(*control).second->getRoot()->getVisible())
			{
				(*control).second->setProperty(_property);
				(*control).second->getRoot()->setVisible(true);

				return;
			}
		}

		if (components::FactoryManager::GetInstance().ExistFactory(_property->getType()->getType()))
		{
			PropertyControl* control = dynamic_cast<PropertyControl*>(components::FactoryManager::GetInstance().CreateItem(_property->getType()->getType()));
			if (control != nullptr)
			{
				control->Initialise(this, mMainWidget, "");

				mPropertyControls.push_back(std::make_pair(_property->getType()->getType(), control));
				control->setProperty(_property);
			}
		}
	}

}
