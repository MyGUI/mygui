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
		//mParentTypeName = "Group";
		//mTypeName = "Index";

		ScopeManager::getInstance().eventChangeScope.connect(this, &FrameTextureController::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());
	}

	void FrameTextureController::deactivate()
	{
		ScopeManager::getInstance().eventChangeScope.disconnect(this);
	}

	void FrameTextureController::notifyChangeDataSelector(Data* _data, bool _changeOnlySelection)
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
				Property* property = data->getParent()->getProperties().find("Texture")->second;
				texture = property->getValue();

				if (!property->eventChangeProperty.exist(this, &FrameTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &FrameTextureController::notifyChangeProperty);

				property = data->getParent()->getProperties().find("Size")->second;
				coord = property->getValue();

				if (!property->eventChangeProperty.exist(this, &FrameTextureController::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &FrameTextureController::notifyChangeProperty);

				for (Data::VectorData::const_iterator child = data->getChilds().begin(); child != data->getChilds().end(); child ++)
				{
					property = (*child)->getProperties().find("Point")->second;
					if (!property->eventChangeProperty.exist(this, &FrameTextureController::notifyChangeProperty))
						property->eventChangeProperty.connect(this, &FrameTextureController::notifyChangeProperty);
				}
			}
		}

		mControl->setTextureValue(texture);

		updateCoords(coord);
	}

	void FrameTextureController::notifyChangeProperty(Property* _sender)
	{
		if (mParentData != nullptr &&
			mParentData->getType()->getName() == mParentTypeName)
		{
			if (mParentData == _sender->getOwner())
			{
				if (_sender->getType()->getName() == "Texture")
				{
					mControl->setTextureValue(_sender->getValue());
				}
				else if (_sender->getType()->getName() == "Size")
				{
					updateCoords(_sender->getValue());
				}
			}
			else if (mParentData->getChildSelected() == _sender->getOwner())
			{
				if (_sender->getType()->getName() == "Point")
				{
					updateFrames();
				}
			}
		}
	}

	void FrameTextureController::notifyChangeScope(const std::string& _scope)
	{
		if (_scope == mTypeName)
		{
			if (!mActivated)
			{
				if (mControl != nullptr)
				{
					mControl->clearAll();

					DataSelectorManager::getInstance().getEvent(mParentTypeName)->connect(this, &FrameTextureController::notifyChangeDataSelector);
					mParentData = DataManager::getInstance().getSelectedDataByType(mParentTypeName);
					notifyChangeDataSelector(mParentData, false);

					mControl->getRoot()->setUserString("CurrentScopeController", mParentTypeName);
				}

				mActivated = true;
			}
		}
		else
		{
			if (mActivated)
			{
				if (mControl != nullptr)
				{
					DataSelectorManager::getInstance().getEvent(mParentTypeName)->disconnect(this);
					mParentData = nullptr;

					// мы еще владельцы контрола сбрасываем его
					std::string value = mControl->getRoot()->getUserString("CurrentScopeController");
					if (value == mParentTypeName)
					{
						mControl->getRoot()->setUserString("CurrentScopeController", "");
						notifyChangeDataSelector(mParentData, false);

						mControl->clearAll();
					}
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

		for (VectorCoord::iterator frame = mFrames.begin(); frame != mFrames.end(); frame ++)
		{
			(*frame).width = mSize.width;
			(*frame).height = mSize.height;
		}

		updateFrames();
	}

	void FrameTextureController::updateFrames()
	{
		mFrames.clear();

		Data* selected = mParentData != nullptr ? mParentData->getChildSelected() : nullptr;
		if (selected != nullptr)
		{
			for (Data::VectorData::const_iterator child = selected->getChilds().begin(); child != selected->getChilds().end(); child ++)
			{
				std::string value = (*child)->getPropertyValue("Point");
				mFrames.push_back(MyGUI::IntCoord(MyGUI::IntPoint::parse(value), mSize));
			}
		}

		if (mControl != nullptr)
			mControl->setViewSelectors(mFrames);
	}

}
