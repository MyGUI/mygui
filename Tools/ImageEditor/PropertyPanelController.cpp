/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "PropertyPanelController.h"
#include "FactoryManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "ScopeManager.h"
#include "DataTypeManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(PropertyPanelController)

	PropertyPanelController::PropertyPanelController() :
		mControl(nullptr),
		mParentType(nullptr)
	{
	}

	PropertyPanelController::~PropertyPanelController()
	{
	}

	void PropertyPanelController::setTarget(Control* _control)
	{
		mControl = _control->findControl<PropertyPanelControl>();
	}

	void PropertyPanelController::activate()
	{
		ScopeManager::getInstance().eventChangeScope.connect(this, &PropertyPanelController::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());
	}

	void PropertyPanelController::deactivate()
	{
		ScopeManager::getInstance().eventChangeScope.disconnect(this);
	}

	void PropertyPanelController::notifyChangeScope(const std::string& _scope)
	{
		if (mParentType != nullptr)
		{
			DataSelectorManager::getInstance().getEvent(mParentType->getName())->disconnect(this);
			mParentType = nullptr;
		}

		mParentType = DataTypeManager::getInstance().getParentType(_scope);

		if (mParentType != nullptr)
		{
			DataSelectorManager::getInstance().getEvent(mParentType->getName())->connect(this, &PropertyPanelController::notifyChangeDataSelector);

			Data* parentData = DataManager::getInstance().getSelectedDataByType(mParentType->getName());
			notifyChangeDataSelector(parentData, false);
		}
	}

	void PropertyPanelController::notifyChangeDataSelector(Data* _data, bool _changeOnlySelection)
	{
		if (mControl != nullptr)
			mControl->setCurrentData(_data != nullptr ? _data->getChildSelected() : nullptr);
	}

}
