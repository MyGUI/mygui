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
		mCurrentData(nullptr),
		mDistance(0)
	{
	}

	PropertyPanelControl::~PropertyPanelControl()
	{
	}

	void PropertyPanelControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		mDistance = MyGUI::utility::parseValue<int>(mMainWidget->getUserString("HeightDistance"));

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
		HideControls();

		if (mCurrentData != nullptr)
		{
			mCurrentData = nullptr;
		}

		if (_data != nullptr)
			mCurrentData = _data->getChildSelected();

		if (mCurrentData != nullptr)
		{
			int height = 0;
			const Data::MapString& properties = mCurrentData->getProperties();
			for (Data::MapString::const_iterator property = properties.begin(); property != properties.end(); property ++)
			{
				if ((*property).second->getType()->getVisible())
					InitialiseProperty((*property).second, height);
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

	void PropertyPanelControl::InitialiseProperty(Property* _property, int& _height)
	{
		std::string type = _property->getType()->getType();
		PropertyControl* control = nullptr;

		for (VectorPairControl::iterator child = mPropertyControls.begin(); child != mPropertyControls.end(); child ++)
		{
			if ((*child).first == type && !(*child).second->getRoot()->getVisible())
			{
				control = (*child).second;
				control->getRoot()->setVisible(true);
				break;
			}
		}

		if (control == nullptr && components::FactoryManager::GetInstance().ExistFactory(_property->getType()->getType()))
		{
			control = dynamic_cast<PropertyControl*>(components::FactoryManager::GetInstance().CreateItem(_property->getType()->getType()));
			control->Initialise(this, mMainWidget, "");

			mPropertyControls.push_back(std::make_pair(_property->getType()->getType(), control));
		}

		if (control != nullptr)
		{
			control->setProperty(_property);
			control->getRoot()->setPosition(0, _height);

			_height += control->getRoot()->getHeight() + mDistance;
		}
	}

}
