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

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(PropertyPanelControl)

	PropertyPanelControl::PropertyPanelControl() :
		mText(nullptr),
		//mParentData(nullptr),
		mParentType(nullptr)
	{
	}

	PropertyPanelControl::~PropertyPanelControl()
	{
	}

	void PropertyPanelControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mText, "Text");

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
		//mParentData = _data;

		std::string text = "";
		if (_data != nullptr)
		{
			text += MyGUI::utility::toString(_data->getType()->getName(), "\n\n");

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

		mText->setCaption(text);
	}

}
