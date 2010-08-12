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
		mScaleValue(1)
	{
		mCoordReal = mMainWidget->getCoord();

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowChangeCoord += MyGUI::newDelegate(this, &SelectorControl::notifyWindowChangeCoord);
	}

	SelectorControl::~SelectorControl()
	{
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
		mCoordReal.left = (int)((double)mCoordValue.left * mScaleValue);
		mCoordReal.top = (int)((double)mCoordValue.top * mScaleValue);
		mCoordReal.width = (int)((double)mCoordValue.width * mScaleValue);
		mCoordReal.height = (int)((double)mCoordValue.height * mScaleValue);

		mMainWidget->setCoord(mCoordReal);
	}

	void SelectorControl::notifyWindowChangeCoord(MyGUI::Window* _sender)
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

} // namespace tools
