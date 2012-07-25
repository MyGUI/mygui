/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "RegionTextureController.h"
#include "FactoryManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "PropertyUtility.h"
#include "ScopeManager.h"
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(RegionTextureController)

	RegionTextureController::RegionTextureController() :
		mControl(nullptr),
		mParentData(nullptr),
		mActivated(false)
	{
	}

	RegionTextureController::~RegionTextureController()
	{
	}

	void RegionTextureController::setTarget(Control* _control)
	{
		mControl = _control->findControl<ScopeTextureControl>();
	}

	void RegionTextureController::activate()
	{
		mParentTypeName = "Skin";
		mThisType = "Region";
		mScopeName = "Region";

		ScopeManager::getInstance().eventChangeScope.connect(this, &RegionTextureController::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());
	}

	void RegionTextureController::deactivate()
	{
		ScopeManager::getInstance().eventChangeScope.disconnect(this);
	}

	void RegionTextureController::notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection)
	{
		mParentData = _data;
		if (mParentData != nullptr && mParentData->getType()->getName() != mParentTypeName)
			mParentData = nullptr;

		std::string texture;
		PropertyPtr property = PropertyUtility::getPropertyByName("Skin", "Texture");
		if (property != nullptr)
		{
			texture = property->getValue();

			if (!property->eventChangeProperty.exist(this, &RegionTextureController::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &RegionTextureController::notifyChangeProperty);
		}

		std::string coord;
		property = PropertyUtility::getPropertyByName("Skin", "Size");
		if (property != nullptr)
		{
			coord = property->getValue();

			if (!property->eventChangeProperty.exist(this, &RegionTextureController::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &RegionTextureController::notifyChangeProperty);
		}

		if (mParentData != nullptr)
		{
			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin(); child != mParentData->getChilds().end(); child ++)
			{
				if ((*child)->getType()->getName() != mThisType && (*child)->getType()->getFriend() != mThisType)
					continue;

				property = (*child)->getProperty("Coord");
				if (!property->eventChangeProperty.exist(this, &RegionTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &RegionTextureController::notifyChangeProperty);

				property = (*child)->getProperty("Visible");
				if (!property->eventChangeProperty.exist(this, &RegionTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &RegionTextureController::notifyChangeProperty);

				property = (*child)->getProperty("Enable");
				if (!property->eventChangeProperty.exist(this, &RegionTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &RegionTextureController::notifyChangeProperty);
			}
		}

		mControl->setTextureValue(texture);
		updateCoords(coord);
	}

	void RegionTextureController::notifyChangeProperty(PropertyPtr _sender)
	{
		if (!mActivated)
			return;

		if (_sender->getOwner()->getType()->getName() == "Skin")
		{
			if (_sender->getType()->getName() == "Texture")
				mControl->setTextureValue(_sender->getValue());
			else if (_sender->getType()->getName() == "Size")
				updateCoords(_sender->getValue());
		}
		else if (_sender->getOwner()->getType()->getName() == mThisType ||
			_sender->getOwner()->getType()->getFriend() == mThisType)
		{
			if (_sender->getType()->getName() == "Coord")
				updateFrames();
			else if (_sender->getType()->getName() == "Visible")
				updateFrames();
			else if (_sender->getType()->getName() == "Enable")
				updateFrames();
		}
	}

	void RegionTextureController::notifyChangeValue(const std::string& _value)
	{
		if (mParentData != nullptr)
		{
			DataPtr selected = mParentData->getChildSelected();
			if (selected != nullptr && selected->getType()->getName() == "RegionText")
			{
				MyGUI::IntCoord coord = MyGUI::IntCoord::parse(_value);
				PropertyPtr property = selected->getProperty("Coord");
				PropertyUtility::executeAction(property, coord.print(), true);
			}
		}
	}

	void RegionTextureController::notifyChangeScope(const std::string& _scope)
	{
		if (mControl == nullptr)
			return;

		if (_scope == mScopeName)
		{
			if (!mActivated)
			{
				mControl->eventChangeValue.connect(this, &RegionTextureController::notifyChangeValue);
				mControl->clearAll();

				DataSelectorManager::getInstance().getEvent(mParentTypeName)->connect(this, &RegionTextureController::notifyChangeDataSelector);
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

	void RegionTextureController::updateCoords(const std::string& _value)
	{
		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(_value, coord.left, coord.top, coord.width, coord.height))
			mTextureCoord = coord;
		else
			mTextureCoord.clear();

		mControl->setTextureRegion(mTextureCoord);

		updateFrames();
	}

	void RegionTextureController::updateFrames()
	{
		mFrames.clear();

		if (mParentData != nullptr)
		{
			DataPtr selected = mParentData->getChildSelected();
			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin(); child != mParentData->getChilds().end(); child ++)
			{
				if ((*child)->getType()->getName() != mThisType && (*child)->getType()->getFriend() != mThisType)
					continue;

				bool visible = (*child)->getPropertyValue<bool>("Visible") &&
					(*child)->getPropertyValue<bool>("Enable");
				MyGUI::IntCoord value = (*child)->getPropertyValue<MyGUI::IntCoord>("Coord");

				if (selected == *child)
				{
					if (visible)
					{
						if ((*child)->getType()->getName() == mThisType)
							mControl->setCoordValue(value, ScopeTextureControl::SelectorPositionReadOnly);
						else
							mControl->setCoordValue(value, ScopeTextureControl::SelectorCoord);
					}
					else
					{
						mControl->clearCoordValue();
					}
				}
				else
				{
					if (visible)
						mFrames.push_back(std::make_pair(value, ScopeTextureControl::SelectorPosition));
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
