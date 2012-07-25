/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "GroupTextureController.h"
#include "FactoryManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "PropertyUtility.h"
#include "ScopeManager.h"
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(GroupTextureController)

	GroupTextureController::GroupTextureController() :
		mControl(nullptr),
		mParentData(nullptr),
		mActivated(false)
	{
	}

	GroupTextureController::~GroupTextureController()
	{
	}

	void GroupTextureController::setTarget(Control* _control)
	{
		mControl = _control->findControl<ScopeTextureControl>();
	}

	void GroupTextureController::activate()
	{
		mParentTypeName = "Image";
		mScopeName = "Group";

		ScopeManager::getInstance().eventChangeScope.connect(this, &GroupTextureController::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());
	}

	void GroupTextureController::deactivate()
	{
		ScopeManager::getInstance().eventChangeScope.disconnect(this);
	}

	void GroupTextureController::notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection)
	{
		mParentData = _data;
		if (mParentData != nullptr && mParentData->getType()->getName() != mParentTypeName)
			mParentData = nullptr;

		std::string texture;
		PropertyPtr property = PropertyUtility::getPropertyByName("Group", "Texture");
		if (property != nullptr)
		{
			texture = property->getValue();

			if (!property->eventChangeProperty.exist(this, &GroupTextureController::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &GroupTextureController::notifyChangeProperty);
		}

		std::string coord;
		property = PropertyUtility::getPropertyByName("Group", "Size");
		if (property != nullptr)
		{
			coord = property->getValue();

			if (!property->eventChangeProperty.exist(this, &GroupTextureController::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &GroupTextureController::notifyChangeProperty);
		}

		mControl->setTextureValue(texture);
		updateCoords(coord);
	}

	void GroupTextureController::notifyChangeProperty(PropertyPtr _sender)
	{
		if (!mActivated || !PropertyUtility::isDataSelected(_sender->getOwner()))
			return;

		if (_sender->getOwner()->getType()->getName() == "Group")
		{
			if (_sender->getType()->getName() == "Texture")
				mControl->setTextureValue(_sender->getValue());
			else if (_sender->getType()->getName() == "Size")
				updateCoords(_sender->getValue());
		}
	}

	void GroupTextureController::notifyChangeValue(const std::string& _value)
	{
		PropertyPtr property = PropertyUtility::getPropertyByName("Group", "Size");
		if (property != nullptr)
			PropertyUtility::executeAction(property, _value, true);
	}

	void GroupTextureController::notifyChangeScope(const std::string& _scope)
	{
		if (mControl == nullptr)
			return;

		if (_scope == mScopeName)
		{
			if (!mActivated)
			{
				mControl->eventChangeValue.connect(this, &GroupTextureController::notifyChangeValue);
				mControl->clearAll();

				DataSelectorManager::getInstance().getEvent(mParentTypeName)->connect(this, &GroupTextureController::notifyChangeDataSelector);
				mParentData = DataUtility::getSelectedDataByType(mParentTypeName);
				notifyChangeDataSelector(mParentData, false);

				mControl->getRoot()->setUserString("CurrentScopeController", mScopeName);

				mActivated = true;
			}
		}
		else
		{
			if (mActivated)
			{
				mControl->eventChangeValue.disconnect(this);

				DataSelectorManager::getInstance().getEvent(mParentTypeName)->disconnect(this);
				mParentData = nullptr;

				// мы еще владельцы контрола сбрасываем его
				std::string value = mControl->getRoot()->getUserString("CurrentScopeController");
				if (value == mScopeName)
				{
					mControl->getRoot()->setUserString("CurrentScopeController", "");
					notifyChangeDataSelector(mParentData, false);

					mControl->clearAll();
				}

				mActivated = false;
			}
		}
	}

	void GroupTextureController::updateCoords(const std::string& _value)
	{
		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(_value, coord.left, coord.top, coord.width, coord.height))
		{
			mControl->setCoordValue(coord, ScopeTextureControl::SelectorCoord);
		}
		else
		{
			mControl->clearCoordValue();
		}
	}

}
