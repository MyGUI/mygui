/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "StateTextureController.h"
#include "FactoryManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "PropertyUtility.h"
#include "ScopeManager.h"
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(StateTextureController)

	StateTextureController::StateTextureController() :
		mControl(nullptr),
		mParentData(nullptr),
		mActivated(false)
	{
	}

	StateTextureController::~StateTextureController()
	{
	}

	void StateTextureController::setTarget(Control* _control)
	{
		mControl = _control->findControl<ScopeTextureControl>();
	}

	void StateTextureController::activate()
	{
		mParentTypeName = "Skin";
		mScopeName = "State";

		ScopeManager::getInstance().eventChangeScope.connect(this, &StateTextureController::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());
	}

	void StateTextureController::deactivate()
	{
		ScopeManager::getInstance().eventChangeScope.disconnect(this);
	}

	void StateTextureController::notifyChangeDataSelector(Data* _data, bool _changeOnlySelection)
	{
		mParentData = _data;
		if (mParentData != nullptr && mParentData->getType()->getName() != mParentTypeName)
			mParentData = nullptr;

		std::string texture;
		Property* property = PropertyUtility::getPropertyByName("Skin", "Texture");
		if (property != nullptr)
		{
			texture = property->getValue();

			if (!property->eventChangeProperty.exist(this, &StateTextureController::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &StateTextureController::notifyChangeProperty);
		}

		std::string coord;
		property = PropertyUtility::getPropertyByName("Skin", "Size");
		if (property != nullptr)
		{
			coord = property->getValue();

			if (!property->eventChangeProperty.exist(this, &StateTextureController::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &StateTextureController::notifyChangeProperty);
		}

		if (mParentData != nullptr)
		{
			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin(); child != mParentData->getChilds().end(); child ++)
			{
				property = (*child)->getProperty("Point");
				if (!property->eventChangeProperty.exist(this, &StateTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &StateTextureController::notifyChangeProperty);
			}
		}

		mControl->setTextureValue(texture);
		updateCoords(coord);
	}

	void StateTextureController::notifyChangeProperty(Property* _sender)
	{
		if (!PropertyUtility::isDataSelected(_sender->getOwner()))
			return;

		if (_sender->getOwner()->getType()->getName() == "Skin")
		{
			if (_sender->getType()->getName() == "Texture")
				mControl->setTextureValue(_sender->getValue());
			else if (_sender->getType()->getName() == "Size")
				updateCoords(_sender->getValue());
		}
		else if (_sender->getOwner()->getType()->getName() == "State")
		{
			if (_sender->getType()->getName() == "Point")
				updateFrames();
		}
	}

	void StateTextureController::notifyChangeValue(const std::string& _value)
	{
		if (mParentData != nullptr)
		{
			Data* selected = mParentData->getChildSelected();
			if (selected != nullptr)
			{
				MyGUI::IntCoord coord = MyGUI::IntCoord::parse(_value);
				Property* property = selected->getProperty("Point");
				PropertyUtility::executeAction(property, coord.point().print(), true);
			}
		}
	}

	void StateTextureController::notifyChangeScope(const std::string& _scope)
	{
		if (mControl == nullptr)
			return;

		if (_scope == mScopeName)
		{
			if (!mActivated)
			{
				mControl->eventChangeValue.connect(this, &StateTextureController::notifyChangeValue);
				mControl->clearAll();

				mControl->setActiveSelector(true);

				DataSelectorManager::getInstance().getEvent(mParentTypeName)->connect(this, &StateTextureController::notifyChangeDataSelector);
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

	void StateTextureController::updateCoords(const std::string& _value)
	{
		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(_value, coord.left, coord.top, coord.width, coord.height))
			mSize = coord.size();
		else
			mSize.clear();

		updateFrames();
	}

	void StateTextureController::updateFrames()
	{
		mFrames.clear();

		if (mParentData != nullptr)
		{
			Data* selected = mParentData->getChildSelected();
			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin(); child != mParentData->getChilds().end(); child ++)
			{
				if (selected == *child)
				{
					MyGUI::IntPoint value = (*child)->getPropertyValue<MyGUI::IntPoint>("Point");
					mControl->setCoordValue(MyGUI::IntCoord(value, mSize));
				}
				else
				{
					MyGUI::IntPoint value = (*child)->getPropertyValue<MyGUI::IntPoint>("Point");
					mFrames.push_back(MyGUI::IntCoord(value, mSize));
				}
			}

			if (selected == nullptr)
			{
				mControl->clearCoordValue();
			}
		}

		if (mControl != nullptr)
			mControl->setViewSelectors(mFrames);
	}

}
