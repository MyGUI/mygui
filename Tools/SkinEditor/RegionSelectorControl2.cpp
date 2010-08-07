/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "RegionSelectorControl2.h"

namespace tools
{

	RegionSelectorControl2::RegionSelectorControl2(MyGUI::Widget* _parent) :
		wraps::BaseLayout("RegionSelectorControl2.layout", _parent),
		mScaleValue(1)
	{
		mCoordReal = mMainWidget->getCoord();

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowChangeCoord += MyGUI::newDelegate(this, &RegionSelectorControl2::notifyWindowChangeCoord);
	}

	RegionSelectorControl2::~RegionSelectorControl2()
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowChangeCoord -= MyGUI::newDelegate(this, &RegionSelectorControl2::notifyWindowChangeCoord);
	}

	void RegionSelectorControl2::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	void RegionSelectorControl2::setSize(const MyGUI::IntSize& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void RegionSelectorControl2::setPosition(const MyGUI::IntPoint& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void RegionSelectorControl2::setScale(double _value)
	{
		mScaleValue = _value;
		updateCoord();
	}

	void RegionSelectorControl2::updateCoord()
	{
		mCoordReal.left = (int)((double)mCoordValue.left * mScaleValue);
		mCoordReal.top = (int)((double)mCoordValue.top * mScaleValue);
		mCoordReal.width = (int)((double)mCoordValue.width * mScaleValue);
		mCoordReal.height = (int)((double)mCoordValue.height * mScaleValue);

		mMainWidget->setCoord(mCoordReal);
	}

	void RegionSelectorControl2::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		MyGUI::IntCoord coord = _sender->getCoord();

		mCoordValue.left = (int)((double)coord.left / mScaleValue);
		mCoordValue.top = (int)((double)coord.top / mScaleValue);

		if (mCoordReal.left != coord.left && mCoordReal.width != coord.width)
			mCoordValue.width = (int)((double)coord.right() / mScaleValue) - mCoordValue.left;
		else
			mCoordValue.width = (int)((double)coord.width / mScaleValue);

		if (mCoordReal.top != coord.top && mCoordReal.height != coord.height)
			mCoordValue.height = (int)((double)coord.bottom() / mScaleValue) - mCoordValue.top;
		else
			mCoordValue.height = (int)((double)coord.height / mScaleValue);

		updateCoord();

		eventChangePosition();
	}

	MyGUI::IntPoint RegionSelectorControl2::getPosition()
	{
		return mCoordValue.point();
	}

} // namespace tools
