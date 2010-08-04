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
		int left = mCoordValue.left * (int)mScaleValue;
		int top = mCoordValue.top * (int)mScaleValue;
		int width = mCoordValue.width * (int)mScaleValue;
		int height = mCoordValue.height * (int)mScaleValue;

		mMainWidget->setCoord(MyGUI::IntCoord(left, top, width, height));
	}

	void RegionSelectorControl::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		MyGUI::IntCoord coord = _sender->getCoord();

		/*double left = (int)((double)coord.left / mScaleValue);
		double top = (int)((double)coord.top / mScaleValue);
		double right = (int)((double)coord.right() / mScaleValue);
		double bottom = (int)((double)coord.bottom() / mScaleValue);

		mCoordValue = MyGUI::IntCoord((int)left, (int)top, (int)(right - left), (int)(bottom - top));

		left = left * mScaleValue;
		top = top * mScaleValue;
		right = right * mScaleValue;
		bottom = bottom * mScaleValue;

		mMainWidget->setCoord(MyGUI::IntCoord((int)left, (int)top, (int)(right - left), (int)(bottom - top)));*/

		double left = (int)((double)coord.left / mScaleValue);
		double top = (int)((double)coord.top / mScaleValue);
		double width = (int)((double)coord.width / mScaleValue);
		double height = (int)((double)coord.height / mScaleValue);

		mCoordValue = MyGUI::IntCoord((int)left, (int)top, (int)width, (int)height);

		left = left * mScaleValue;
		top = top * mScaleValue;
		width = width * mScaleValue;
		height = height * mScaleValue;

		mMainWidget->setCoord(MyGUI::IntCoord((int)left, (int)top, (int)width, (int)height));

		eventChangePosition();
	}

	const MyGUI::IntCoord& RegionSelectorControl::getCoord()
	{
		return mCoordValue;
	}

} // namespace tools
