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

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(GroupTextureController)

	GroupTextureController::GroupTextureController() :
		mControl(nullptr),
		mParentData(nullptr)
	{
	}

	GroupTextureController::~GroupTextureController()
	{
	}

	void GroupTextureController::setTarget(Control* _control)
	{
		mControl = dynamic_cast<ScopeTextureControl*>(_control);
	}

	void GroupTextureController::activate()
	{
		mParentTypeName = "Image";

		if (mControl != nullptr)
		{
			DataSelectorManager::getInstance().getEvent(mParentTypeName)->connect(this, &GroupTextureController::notifyChangeDataSelector);
			mParentData = DataManager::getInstance().getSelectedDataByType(mParentTypeName);
			notifyChangeDataSelector(mParentData, false);

			mControl->eventChangeValue.connect(this, &GroupTextureController::notifyChangeValue);
		}
	}

	void GroupTextureController::diactivate()
	{
		if (mControl != nullptr)
		{
			mControl->eventChangeValue.disconnect(this);

			DataSelectorManager::getInstance().getEvent(mParentTypeName)->disconnect(this);
			mParentData = nullptr;
			notifyChangeDataSelector(mParentData, false);
		}
	}

	void GroupTextureController::notifyChangeDataSelector(Data* _data, bool _changeOnlySelection)
	{
		mParentData = _data;
		if (mParentData != nullptr && mParentData->getType()->getName() != mParentTypeName)
			mParentData = nullptr;

		std::string texture;
		std::string coord;

		if (mParentData != nullptr)
		{
			Data* data = mParentData->getChildSelected();
			if (data != nullptr)
			{
				Property* property = data->getProperties().find("Texture")->second;
				texture = property->getValue();

				if (!property->eventChangeProperty.compare(this, &GroupTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &GroupTextureController::notifyChangeProperty);

				property = data->getProperties().find("Size")->second;
				coord = property->getValue();

				if (!property->eventChangeProperty.compare(this, &GroupTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &GroupTextureController::notifyChangeProperty);
			}
		}

		mControl->setTextureValue(texture);
		mControl->setCoordValue(coord);
	}

	void GroupTextureController::notifyChangeProperty(Property* _sender)
	{
		if (mParentData != nullptr &&
			mParentData->getType()->getName() == mParentTypeName &&
			mParentData->getChildSelected() == _sender->getOwner())
		{
			if (_sender->getType()->getName() == "Texture")
				mControl->setTextureValue(_sender->getValue());
			else if (_sender->getType()->getName() == "Size")
				mControl->setCoordValue(_sender->getValue());
		}
	}

	void GroupTextureController::notifyChangeValue(const std::string& _value)
	{
		if (mParentData != nullptr &&
			mParentData->getType()->getName() == mParentTypeName)
		{
			Data* selected = mParentData->getChildSelected();
			if (selected != nullptr)
			{
				Property* property = selected->getProperties().find("Size")->second;
				PropertyUtility::executeAction(property, _value, true);
			}
		}
	}

}
