/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "FrameTextureController.h"
#include "FactoryManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "PropertyUtility.h"
#include "ScopeManager.h"
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(FrameTextureController)

	FrameTextureController::FrameTextureController() :
		mControl(nullptr),
		mParentData(nullptr),
		mActivated(false)
	{
	}

	FrameTextureController::~FrameTextureController()
	{
	}

	void FrameTextureController::setTarget(Control* _control)
	{
		mControl = _control->findControl<ScopeTextureControl>();
	}

	void FrameTextureController::activate()
	{
		mParentTypeName = "Index";
		mScopeName = "Frame";

		ScopeManager::getInstance().eventChangeScope.connect(this, &FrameTextureController::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());
	}

	void FrameTextureController::deactivate()
	{
		ScopeManager::getInstance().eventChangeScope.disconnect(this);
	}

	void FrameTextureController::notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection)
	{
		mParentData = _data;
		if (mParentData != nullptr && mParentData->getType()->getName() != mParentTypeName)
			mParentData = nullptr;

		std::string texture;
		PropertyPtr property = PropertyUtility::getPropertyByName("Group", "Texture");
		if (property != nullptr)
		{
			texture = property->getValue();

			if (!property->eventChangeProperty.exist(this, &FrameTextureController::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &FrameTextureController::notifyChangeProperty);
		}

		std::string coord;
		property = PropertyUtility::getPropertyByName("Group", "Size");
		if (property != nullptr)
		{
			coord = property->getValue();

			if (!property->eventChangeProperty.exist(this, &FrameTextureController::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &FrameTextureController::notifyChangeProperty);
		}

		if (mParentData != nullptr)
		{
			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin(); child != mParentData->getChilds().end(); child ++)
			{
				property = (*child)->getProperty("Point");
				if (!property->eventChangeProperty.exist(this, &FrameTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &FrameTextureController::notifyChangeProperty);
			}
		}

		mControl->setTextureValue(texture);
		updateCoords(coord);
	}

	void FrameTextureController::notifyChangeProperty(PropertyPtr _sender)
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
		else if (_sender->getOwner()->getType()->getName() == "Frame")
		{
			if (_sender->getType()->getName() == "Point")
				updateFrames();
		}
	}

	void FrameTextureController::notifyChangeValue(const std::string& _value)
	{
		if (mParentData != nullptr)
		{
			DataPtr selected = mParentData->getChildSelected();
			if (selected != nullptr)
			{
				MyGUI::IntCoord coord = MyGUI::IntCoord::parse(_value);
				PropertyPtr property = selected->getProperty("Point");
				PropertyUtility::executeAction(property, coord.point().print(), true);
			}
		}
	}

	void FrameTextureController::notifyChangeScope(const std::string& _scope)
	{
		if (mControl == nullptr)
			return;

		if (_scope == mScopeName)
		{
			if (!mActivated)
			{
				mControl->eventChangeValue.connect(this, &FrameTextureController::notifyChangeValue);
				mControl->clearAll();

				DataSelectorManager::getInstance().getEvent(mParentTypeName)->connect(this, &FrameTextureController::notifyChangeDataSelector);
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

	void FrameTextureController::updateCoords(const std::string& _value)
	{
		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(_value, coord.left, coord.top, coord.width, coord.height))
			mSize = coord.size();
		else
			mSize.clear();

		updateFrames();
	}

	void FrameTextureController::updateFrames()
	{
		mFrames.clear();

		if (mParentData != nullptr)
		{
			DataPtr selected = mParentData->getChildSelected();
			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin(); child != mParentData->getChilds().end(); child ++)
			{
				if (selected == *child)
				{
					MyGUI::IntPoint value = (*child)->getPropertyValue<MyGUI::IntPoint>("Point");
					mControl->setCoordValue(MyGUI::IntCoord(value, mSize), ScopeTextureControl::SelectorPosition);
				}
				else
				{
					MyGUI::IntPoint value = (*child)->getPropertyValue<MyGUI::IntPoint>("Point");
					mFrames.push_back(std::make_pair(MyGUI::IntCoord(value, mSize), ScopeTextureControl::SelectorPosition));
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
