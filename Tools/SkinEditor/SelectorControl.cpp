/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SelectorControl.h"

namespace tools
{

	SelectorControl::SelectorControl(const std::string& _layout, MyGUI::Widget* _parent) :
		wraps::BaseLayout(_layout, _parent),
		mScaleValue(1.0),
		mPositionChanged(false)
	{
		assignWidget(mProjection, "Projection", false);

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

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &SelectorControl::notifyFrameStart);
	}

	SelectorControl::~SelectorControl()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &SelectorControl::notifyFrameStart);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowChangeCoord -= MyGUI::newDelegate(this, &SelectorControl::notifyWindowChangeCoord);
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
		mCoordReal.left = (int)((double)mCoordValue.left * mScaleValue) + mProjectionDiff.left;
		mCoordReal.top = (int)((double)mCoordValue.top * mScaleValue) + mProjectionDiff.top;
		mCoordReal.width = (int)((double)mCoordValue.width * mScaleValue) + mProjectionDiff.width;
		mCoordReal.height = (int)((double)mCoordValue.height * mScaleValue) + mProjectionDiff.height;

		mMainWidget->setCoord(mCoordReal);
	}

	void SelectorControl::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		mPositionChanged = true;

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
	}

	MyGUI::IntPoint SelectorControl::getPosition()
	{
		return mCoordValue.point();
	}

	MyGUI::IntSize SelectorControl::getSize()
	{
		return mCoordValue.size();
	}

	const MyGUI::IntCoord& SelectorControl::getCoord()
	{
		return mCoordValue;
	}

	void SelectorControl::setEnabled(bool _value)
	{
		mMainWidget->setNeedMouseFocus(_value);
	}

	void SelectorControl::notifyFrameStart(float _time)
	{
		if (mPositionChanged)
		{
			eventChangePosition();
			mPositionChanged = false;
		}
	}

} // namespace tools
