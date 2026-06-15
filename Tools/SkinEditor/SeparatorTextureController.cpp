/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "SeparatorTextureController.h"
#include "FactoryManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "PropertyUtility.h"
#include "ScopeManager.h"
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SeparatorTextureController)

	void SeparatorTextureController::setTarget(Control* _control)
	{
		mControl = _control->findControl<ScopeTextureControl>();
	}

	void SeparatorTextureController::activate()
	{
		mParentTypeName = "Skin";
		mThisType = "Separator";
		mScopeName = "Separator";

		ScopeManager::getInstance().eventChangeScope.connect(this, &SeparatorTextureController::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());
	}

	void SeparatorTextureController::deactivate()
	{
		ScopeManager::getInstance().eventChangeScope.disconnect(this);
	}

	void SeparatorTextureController::notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection)
	{
		mParentData = _data;
		if (mParentData != nullptr && mParentData->getType()->getName() != mParentTypeName)
			mParentData = nullptr;

		std::string_view texture;
		PropertyPtr property = PropertyUtility::getPropertyByName("Skin", "Texture");
		if (property != nullptr)
		{
			texture = property->getValue();

			if (!property->eventChangeProperty.exist(this, &SeparatorTextureController::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &SeparatorTextureController::notifyChangeProperty);
		}

		std::string_view coord;
		property = PropertyUtility::getPropertyByName("Skin", "Size");
		if (property != nullptr)
		{
			coord = property->getValue();

			if (!property->eventChangeProperty.exist(this, &SeparatorTextureController::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &SeparatorTextureController::notifyChangeProperty);
		}

		if (mParentData != nullptr)
		{
			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin();
				 child != mParentData->getChilds().end();
				 child++)
			{
				if ((*child)->getType()->getName() != mThisType)
					continue;

				property = (*child)->getProperty("Offset");
				if (!property->eventChangeProperty.exist(this, &SeparatorTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &SeparatorTextureController::notifyChangeProperty);

				property = (*child)->getProperty("Visible");
				if (!property->eventChangeProperty.exist(this, &SeparatorTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &SeparatorTextureController::notifyChangeProperty);
			}
		}

		mControl->setTextureValue(MyGUI::UString(texture));
		updateCoords(coord);
	}

	void SeparatorTextureController::notifyChangeProperty(PropertyPtr _sender)
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
		else if (_sender->getOwner()->getType()->getName() == "Separator")
		{
			if (_sender->getType()->getName() == "Offset")
				updateFrames();
			else if (_sender->getType()->getName() == "Visible")
				updateFrames();
		}
	}

	void SeparatorTextureController::notifyChangeValue(std::string_view _value)
	{
		if (mParentData != nullptr)
		{
			DataPtr selected = mParentData->getChildSelected();
			if (selected != nullptr && selected->getType()->getName() == "Separator")
			{
				MyGUI::IntCoord value = MyGUI::IntCoord::parse(_value);
				int offset = getOffsetByName(value, selected->getPropertyValue("Name"));
				PropertyPtr property = selected->getProperty("Offset");
				PropertyUtility::executeAction(property, MyGUI::utility::toString(offset), true);
			}
		}
	}

	void SeparatorTextureController::notifyChangeScope(std::string_view _scope)
	{
		if (mControl == nullptr)
			return;

		if (_scope == mScopeName)
		{
			if (!mActivated)
			{
				mControl->eventChangeValue.connect(this, &SeparatorTextureController::notifyChangeValue);
				mControl->clearAll();

				DataSelectorManager::getInstance()
					.getEvent(mParentTypeName)
					->connect(this, &SeparatorTextureController::notifyChangeDataSelector);
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
				std::string_view value = mControl->getRoot()->getUserString("CurrentScopeController");
				if (value == mScopeName)
				{
					mControl->getRoot()->setUserString("CurrentScopeController", std::string_view{});
					notifyChangeDataSelector(mParentData, false);

					mControl->clearAll();
				}

				mActivated = false;
			}
		}
	}

	void SeparatorTextureController::updateCoords(std::string_view _value)
	{
		MyGUI::IntCoord coord;
		if (MyGUI::utility::parseComplex(_value, coord.left, coord.top, coord.width, coord.height))
			mTextureCoord = coord;
		else
			mTextureCoord.clear();

		mControl->setTextureRegion(mTextureCoord);

		updateFrames();
	}

	void SeparatorTextureController::updateFrames()
	{
		mFrames.clear();

		if (mParentData != nullptr)
		{
			DataPtr selected = mParentData->getChildSelected();
			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin();
				 child != mParentData->getChilds().end();
				 child++)
			{
				if ((*child)->getType()->getName() != mThisType)
					continue;

				bool visible = (*child)->getPropertyValue<bool>("Visible");
				int offset = (*child)->getPropertyValue<int>("Offset");

				std::string name = (*child)->getPropertyValue("Name");
				MyGUI::IntCoord value = getCoordByName(name, offset);
				ScopeTextureControl::SelectorType type = getTypeByName(name);

				if (selected == *child)
				{
					if (visible)
						mControl->setCoordValue(value, type);
					else
						mControl->clearCoordValue();
				}
				else
				{
					if (visible)
					{
						mFrames.emplace_back(value, type);
					}
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

	ScopeTextureControl::SelectorType SeparatorTextureController::getTypeByName(std::string_view _name)
	{
		if (_name == "Left" || _name == "Right")
			return ScopeTextureControl::SelectorOffsetV;
		if (_name == "Top" || _name == "Bottom")
			return ScopeTextureControl::SelectorOffsetH;

		return ScopeTextureControl::SelectorPosition;
	}

	MyGUI::IntCoord SeparatorTextureController::getCoordByName(std::string_view _name, int _offset) const
	{
		if (_name == "Left")
			return {_offset, 0, 1, mTextureCoord.height};
		if (_name == "Right")
			return {mTextureCoord.width - _offset, 0, 1, mTextureCoord.height};
		if (_name == "Top")
			return {0, _offset, mTextureCoord.width, 1};
		if (_name == "Bottom")
			return {0, mTextureCoord.height - _offset, mTextureCoord.width, 1};

		return {};
	}

	int SeparatorTextureController::getOffsetByName(const MyGUI::IntCoord& _coord, std::string_view _name) const
	{
		if (_name == "Left")
			return _coord.left;
		if (_name == "Right")
			return mTextureCoord.width - _coord.left;
		if (_name == "Top")
			return _coord.top;
		if (_name == "Bottom")
			return mTextureCoord.height - _coord.top;

		return 0;
	}

}
