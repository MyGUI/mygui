/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "ListBoxControl.h"
#include "SettingsManager.h"

namespace tools
{

	ListBoxControl::ListBoxControl(const std::string& _layout, MyGUI::Widget* _parent) :
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
			window->eventWindowChangeCoord += MyGUI::newDelegate(this, &ListBoxControl::notifyWindowChangeCoord);

		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &ListBoxControl::notifySettingsChanged);
	}

	ListBoxControl::~ListBoxControl()
	{
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &ListBoxControl::notifySettingsChanged);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowChangeCoord -= MyGUI::newDelegate(this, &ListBoxControl::notifyWindowChangeCoord);
	}

	void ListBoxControl::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	void ListBoxControl::setSize(const MyGUI::IntSize& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void ListBoxControl::setPosition(const MyGUI::IntPoint& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void ListBoxControl::setCoord(const MyGUI::IntCoord& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void ListBoxControl::setScale(double _value)
	{
		mScaleValue = _value;
		updateCoord();
	}

	void ListBoxControl::updateCoord()
	{
		mCoordReal.left = (int)((double)mCoordValue.left * mScaleValue) + mProjectionDiff.left;
		mCoordReal.top = (int)((double)mCoordValue.top * mScaleValue) + mProjectionDiff.top;
		mCoordReal.width = (int)((double)mCoordValue.width * mScaleValue) + mProjectionDiff.width;
		mCoordReal.height = (int)((double)mCoordValue.height * mScaleValue) + mProjectionDiff.height;

		mMainWidget->setCoord(mCoordReal);
	}

	void ListBoxControl::notifyWindowChangeCoord(MyGUI::Window* _sender)
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

	MyGUI::IntPoint ListBoxControl::getPosition()
	{
		return mCoordValue.point();
	}

	MyGUI::IntSize ListBoxControl::getSize()
	{
		return mCoordValue.size();
	}

	const MyGUI::IntCoord& ListBoxControl::getCoord() const
	{
		return mCoordValue;
	}

	void ListBoxControl::setEnabled(bool _value)
	{
		mMainWidget->setNeedMouseFocus(_value);
	}

	bool ListBoxControl::getCapture()
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			return !window->getActionScale().empty();
		return false;
	}

	MyGUI::IntCoord ListBoxControl::getActionScale()
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			return window->getActionScale();
		return MyGUI::IntCoord();
	}

	void ListBoxControl::setColour(MyGUI::Colour _value)
	{
		mMainWidget->setColour(_value);
		mMainWidget->setAlpha(_value.alpha);
	}

	void ListBoxControl::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (!mPropertyColour.empty() && _sectorName == "Settings" && _propertyName == mPropertyColour)
		{
			MyGUI::Colour colour = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>(mPropertyColour);
			setColour(colour);
		}
	}

	void ListBoxControl::setPropertyColour(const std::string& _propertyName)
	{
		mPropertyColour = _propertyName;
		MyGUI::Colour colour = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>(mPropertyColour);
		setColour(colour);
	}

	MyGUI::Widget* ListBoxControl::getMainWidget()
	{
		return mMainWidget;
	}

} // namespace tools
