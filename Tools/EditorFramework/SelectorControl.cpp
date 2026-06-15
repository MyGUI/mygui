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

	SelectorControl::~SelectorControl()
	{
		SettingsManager::getInstance().eventSettingsChanged.disconnect(this);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowChangeCoord -= MyGUI::newDelegate(this, &SelectorControl::notifyWindowChangeCoord);
	}

	void SelectorControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

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
			window->eventWindowChangeCoord += MyGUI::newDelegate(this, &SelectorControl::notifyWindowChangeCoord);

		SettingsManager::getInstance().eventSettingsChanged.connect(this, &SelectorControl::notifySettingsChanged);
	}

	void SelectorControl::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	void SelectorControl::setSize(const MyGUI::IntSize& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void SelectorControl::setPosition(const MyGUI::IntPoint& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void SelectorControl::setCoord(const MyGUI::IntCoord& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void SelectorControl::setScale(double _value)
	{
		mScaleValue = _value;
		updateCoord();
	}

	void SelectorControl::updateCoord()
	{
		mCoordReal.left = static_cast<int>(mCoordValue.left * mScaleValue) + mProjectionDiff.left;
		mCoordReal.top = static_cast<int>(mCoordValue.top * mScaleValue) + mProjectionDiff.top;
		mCoordReal.width = static_cast<int>(mCoordValue.width * mScaleValue) + mProjectionDiff.width;
		mCoordReal.height = static_cast<int>(mCoordValue.height * mScaleValue) + mProjectionDiff.height;

		mMainWidget->setCoord(mCoordReal);
	}

	void SelectorControl::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		MyGUI::IntCoord coord = _sender->getCoord() - mProjectionDiff;
		const MyGUI::IntCoord& actionScale = _sender->getActionScale();

		if (actionScale.left != 0 && actionScale.width != 0)
		{
			int right = mCoordValue.right();
			mCoordValue.width = static_cast<int>(coord.width / mScaleValue);
			mCoordValue.left = right - mCoordValue.width;
		}
		else
		{
			mCoordValue.left = static_cast<int>(coord.left / mScaleValue);
			mCoordValue.width = static_cast<int>(coord.width / mScaleValue);
		}

		if (actionScale.top != 0 && actionScale.height != 0)
		{
			int bottom = mCoordValue.bottom();
			mCoordValue.height = static_cast<int>(coord.height / mScaleValue);
			mCoordValue.top = bottom - mCoordValue.height;
		}
		else
		{
			mCoordValue.top = static_cast<int>(coord.top / mScaleValue);
			mCoordValue.height = static_cast<int>(coord.height / mScaleValue);
		}

		updateCoord();
		eventChangePosition(this);
	}

	MyGUI::IntPoint SelectorControl::getPosition()
	{
		return mCoordValue.point();
	}

	MyGUI::IntSize SelectorControl::getSize()
	{
		return mCoordValue.size();
	}

	const MyGUI::IntCoord& SelectorControl::getCoord() const
	{
		return mCoordValue;
	}

	void SelectorControl::setEnabled(bool _value)
	{
		mMainWidget->setNeedMouseFocus(_value);
	}

	bool SelectorControl::getCapture()
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			return !window->getActionScale().empty();
		return false;
	}

	MyGUI::IntCoord SelectorControl::getActionScale()
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			return window->getActionScale();
		return {};
	}

	void SelectorControl::setColour(MyGUI::Colour _value)
	{
		mMainWidget->setColour(_value);
		mMainWidget->setAlpha(_value.alpha);
	}

	void SelectorControl::notifySettingsChanged(std::string_view _path)
	{
		if (!mPropertyColour.empty() && _path == ("Workspace/Colours/" + mPropertyColour))
		{
			MyGUI::Colour colour =
				SettingsManager::getInstance().getValue<MyGUI::Colour>("Workspace/Colours/" + mPropertyColour);
			setColour(colour);
		}
	}

	void SelectorControl::setPropertyColour(std::string_view _propertyName)
	{
		mPropertyColour = _propertyName;
		MyGUI::Colour colour =
			SettingsManager::getInstance().getValue<MyGUI::Colour>("Workspace/Colours/" + mPropertyColour);
		setColour(colour);
	}

	MyGUI::Widget* SelectorControl::getMainWidget()
	{
		return mMainWidget;
	}

	bool SelectorControl::getVisible() const
	{
		return mMainWidget->getVisible();
	}

}
