/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "PropertyPanelController.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "ScopeManager.h"
#include "DataTypeManager.h"
#include "DataUtility.h"

namespace tools
{

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

			DataPtr parentData = DataUtility::getSelectedDataByType(mParentType->getName());
			notifyChangeDataSelector(parentData, false);
		}
	}

	void PropertyPanelController::notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection)
	{
		if (mControl != nullptr)
		{
			DataPtr selected = _data != nullptr ? _data->getChildSelected() : nullptr;

			// выделяем только данные с типом скопа
			if (selected != nullptr)
			{
				if (selected->getType()->getName() != ScopeManager::getInstance().getCurrentScope() &&
					selected->getType()->getFriend() != ScopeManager::getInstance().getCurrentScope())
				selected = nullptr;
			}

			mControl->setCurrentData(selected);
		}
	}

}
