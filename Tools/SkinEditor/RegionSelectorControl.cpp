/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "RegionSelectorControl.h"

namespace tools
{

	RegionSelectorControl::RegionSelectorControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("RegionSelectorControl.layout", _parent),
		mScaleValue(1)
	{
		mCoordReal = mMainWidget->getCoord();

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowChangeCoord += MyGUI::newDelegate(this, &RegionSelectorControl::notifyWindowChangeCoord);
	}

	RegionSelectorControl::~RegionSelectorControl()
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowChangeCoord -= MyGUI::newDelegate(this, &RegionSelectorControl::notifyWindowChangeCoord);
	}

	void RegionSelectorControl::setVisible(bool _value)
	{
		mMainWidget->setVisible(_value);
	}

	void RegionSelectorControl::setCoord(const MyGUI::IntCoord& _value)
	{
		mCoordValue = _value;
		updateCoord();
	}

	void RegionSelectorControl::setScale(double _value)
	{
		mScaleValue = _value;
		updateCoord();
	}

	void RegionSelectorControl::updateCoord()
	{
		mCoordReal.left = (int)((double)mCoordValue.left * mScaleValue);
		mCoordReal.top = (int)((double)mCoordValue.top * mScaleValue);
		mCoordReal.width = (int)((double)mCoordValue.width * mScaleValue);
		mCoordReal.height = (int)((double)mCoordValue.height * mScaleValue);

		mMainWidget->setCoord(mCoordReal);
	}

	void RegionSelectorControl::notifyWindowChangeCoord(MyGUI::Window* _sender)
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

	const MyGUI::IntCoord& RegionSelectorControl::getCoord()
	{
		return mCoordValue;
	}

} // namespace tools
