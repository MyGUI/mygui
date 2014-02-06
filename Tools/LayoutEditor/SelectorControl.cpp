/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "SelectorControl.h"
#include "SettingsManager.h"

namespace tools
{

	SelectorControlLE::SelectorControlLE(const std::string& _layout, MyGUI::Widget* _parent) :
		wraps::BaseLayout(_layout, _parent),
		mScaleValue(1.0)
	{
		assignWidget(mProjection, "Projection", false, false);

		if (mProjection != nullptr)
		{
			mCoordReal = mProjection->getCoord();
			mProjectionDiff = mMainWidget->getAbsoluteCoord() - mProjection->getAbsoluteCoord();
		}
		else
		{
			mCoordReal = mMainWidget->getCoord();
		}

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowChangeCoord += MyGUI::newDelegate(this, &SelectorControlLE::notifyWindowChangeCoord);

		SettingsManager::getInstance().eventSettingsChanged.connect(this, &SelectorControlLE::notifySettingsChanged);
	}

	SelectorControlLE::~SelectorControlLE()
	{
		SettingsManager::getInstance().eventSettingsChanged.disconnect(this);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowChangeCoord -= MyGUI::newDelegate(this, &SelectorControlLE::notifyWindowChangeCoord);
	}

	void SelectorControlLE::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	void SelectorControlLE::setSize(const MyGUI::IntSize& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void SelectorControlLE::setPosition(const MyGUI::IntPoint& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void SelectorControlLE::setCoord(const MyGUI::IntCoord& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void SelectorControlLE::setScale(double _value)
	{
		mScaleValue = _value;
		updateCoord();
	}

	void SelectorControlLE::updateCoord()
	{
		mCoordReal.left = (int)((double)mCoordValue.left * mScaleValue) + mProjectionDiff.left;
		mCoordReal.top = (int)((double)mCoordValue.top * mScaleValue) + mProjectionDiff.top;
		mCoordReal.width = (int)((double)mCoordValue.width * mScaleValue) + mProjectionDiff.width;
		mCoordReal.height = (int)((double)mCoordValue.height * mScaleValue) + mProjectionDiff.height;

		mMainWidget->setCoord(mCoordReal);
	}

	void SelectorControlLE::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		MyGUI::IntCoord coord = _sender->getCoord() - mProjectionDiff;
		const MyGUI::IntCoord& actionScale = _sender->getActionScale();

		if (actionScale.left != 0 && actionScale.width != 0)
		{
			int right = mCoordValue.right();
			mCoordValue.width = (int)((double)coord.width / mScaleValue);
			mCoordValue.left = right - mCoordValue.width;
		}
		else
		{
			mCoordValue.left = (int)((double)coord.left / mScaleValue);
			mCoordValue.width = (int)((double)coord.width / mScaleValue);
		}

		if (actionScale.top != 0 && actionScale.height != 0)
		{
			int bottom = mCoordValue.bottom();
			mCoordValue.height = (int)((double)coord.height / mScaleValue);
			mCoordValue.top = bottom - mCoordValue.height;
		}
		else
		{
			mCoordValue.top = (int)((double)coord.top / mScaleValue);
			mCoordValue.height = (int)((double)coord.height / mScaleValue);
		}

		updateCoord();
		eventChangePosition();
	}

	MyGUI::IntPoint SelectorControlLE::getPosition()
	{
		return mCoordValue.point();
	}

	MyGUI::IntSize SelectorControlLE::getSize()
	{
		return mCoordValue.size();
	}

	const MyGUI::IntCoord& SelectorControlLE::getCoord() const
	{
		return mCoordValue;
	}

	void SelectorControlLE::setEnabled(bool _value)
	{
		mMainWidget->setNeedMouseFocus(_value);
	}

	bool SelectorControlLE::getCapture()
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			return !window->getActionScale().empty();
		return false;
	}

	MyGUI::IntCoord SelectorControlLE::getActionScale() const
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			return window->getActionScale();
		return MyGUI::IntCoord();
	}

	void SelectorControlLE::setColour(MyGUI::Colour _value)
	{
		mMainWidget->setColour(_value);
		mMainWidget->setAlpha(_value.alpha);
	}

	void SelectorControlLE::notifySettingsChanged(const std::string& _path)
	{
		if (!mPropertyColour.empty() && _path == ("Workspace/Colours/" + mPropertyColour))
		{
			MyGUI::Colour colour = SettingsManager::getInstance().getValue<MyGUI::Colour>("Workspace/Colours/" + mPropertyColour);
			setColour(colour);
		}
	}

	void SelectorControlLE::setPropertyColour(const std::string& _propertyName)
	{
		mPropertyColour = _propertyName;
		MyGUI::Colour colour = SettingsManager::getInstance().getValue<MyGUI::Colour>("Workspace/Colours/" + mPropertyColour);
		setColour(colour);
	}

	MyGUI::Widget* SelectorControlLE::getMainWidget()
	{
		return mMainWidget;
	}

}
