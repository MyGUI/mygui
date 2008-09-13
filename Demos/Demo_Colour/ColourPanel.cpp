/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "ColourPanel.h"


ColourPanel::ColourPanel() :
	BaseLayout("ColourPanel.layout")
{
}

void ColourPanel::initialise()
{
	mCurrentColour = Ogre::ColourValue::Green;
	mBaseColour = Ogre::ColourValue::Green;

	loadLayout();

	assignWidget(mColourRect, "widget_ColourRect");
	assignWidget(mColourView, "widget_ColourView");
	assignWidget(mImageColourPicker, "image_Picker");
	assignWidget(mEditRed, "edit_Red");
	assignWidget(mEditGreen, "edit_Green");
	assignWidget(mEditBlue, "edit_Blue");
	assignWidget(mScrollRange, "scroll_Range");
	assignWidget(mImageRange, "image_ColourRange");

	mColourRect->_setTextureName("gradient.png");
	//mColourRect->_setUVSet(MyGUI::FloatRect(0, 0, 1, 1));
	//mColourRect->set

	mColourRect->eventMouseButtonPressed = MyGUI::newDelegate(this, &ColourPanel::notifyMouseButtonPressed);
	mColourRect->eventMouseDrag = MyGUI::newDelegate(this, &ColourPanel::notifyMouseDrag);
	mImageColourPicker->eventMouseDrag = MyGUI::newDelegate(this, &ColourPanel::notifyMouseDrag);
	mScrollRange->eventScrollChangePosition = MyGUI::newDelegate(this, &ColourPanel::notifyScrollChangePosition);

	mEditRed->eventEditTextChange = MyGUI::newDelegate(this, &ColourPanel::notifyEditTextChange);
	mEditGreen->eventEditTextChange = MyGUI::newDelegate(this, &ColourPanel::notifyEditTextChange);
	mEditBlue->eventEditTextChange = MyGUI::newDelegate(this, &ColourPanel::notifyEditTextChange);

	MyGUI::SubWidgetInterface * main = mColourRect->_getSubWidgetMain();
	mRawColourRect = static_cast<MyGUI::RawRect*>(main);
	main = mColourView->_getSubWidgetMain();
	mRawColourView = static_cast<MyGUI::RawRect*>(main);

	mColourRange.push_back(Ogre::ColourValue(1, 0, 0));
	mColourRange.push_back(Ogre::ColourValue(1, 0, 1));
	mColourRange.push_back(Ogre::ColourValue(0, 0, 1));
	mColourRange.push_back(Ogre::ColourValue(0, 1, 1));
	mColourRange.push_back(Ogre::ColourValue(0, 1, 0));
	mColourRange.push_back(Ogre::ColourValue(1, 1, 0));
	mColourRange.push_back(mColourRange[0]);

	updateFirst();
}

void ColourPanel::show()
{
	mMainWidget->show();
}

void ColourPanel::hide()
{
	mMainWidget->hide();
}

void ColourPanel::updateFirst()
{
	notifyScrollChangePosition(null, mScrollRange->getScrollPosition());
	mRawColourRect->setRectColour(Ogre::ColourValue::White, mBaseColour, Ogre::ColourValue::White, Ogre::ColourValue::White);
	mRawColourRect->setRectTexture(MyGUI::FloatPoint(0, 0), MyGUI::FloatPoint(1, 0), MyGUI::FloatPoint(0, 1), MyGUI::FloatPoint(1, 1));

	notifyMouseDrag(null, mImageColourPicker->getAbsoluteLeft() + 10, mImageColourPicker->getAbsoluteTop() + 10);
}

void ColourPanel::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
{
	MyGUI::WidgetPtr parent = mImageColourPicker->getParent();
	MyGUI::IntPoint point(_left - parent->getAbsoluteLeft(), _top - parent->getAbsoluteTop());

	if (point.left < 0) point.left = 0;
	if (point.top < 0) point.top = 0;
	if (point.left > mColourRect->getWidth()) point.left = mColourRect->getWidth();
	if (point.top > mColourRect->getHeight()) point.top = mColourRect->getHeight();

	mImageColourPicker->setPosition(point.left - (mImageColourPicker->getWidth() / 2), point.top - (mImageColourPicker->getHeight() / 2));

	updateFromPoint(point);
}

void ColourPanel::notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
{
	if (_id == MyGUI::MB_Left) notifyMouseDrag(null, _left, _top);
}

void ColourPanel::updateFromPoint(const MyGUI::IntPoint & _point)
{
	// вычисляем цвет по положению курсора Altren 09.2008
	float x = 1. * _point.left / mColourRect->getWidth();
	float y = 1. * _point.top / mColourRect->getHeight();
	if (x > 1) x = 1;
	else if (x < 0) x = 0;
	if (y > 1) y = 1;
	else if (y < 0) y = 0;

	mCurrentColour = (1 - y) * (mBaseColour * x + Ogre::ColourValue::White * (1 - x));
	mRawColourView->setRectColour(mCurrentColour, mCurrentColour, mCurrentColour, mCurrentColour);

	mEditRed->setCaption(MyGUI::utility::toString((int)(mCurrentColour.r * 255)));
	mEditGreen->setCaption(MyGUI::utility::toString((int)(mCurrentColour.g * 255)));
	mEditBlue->setCaption(MyGUI::utility::toString((int)(mCurrentColour.b * 255)));

}

void ColourPanel::notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _position)
{
	float sector_size = (float)mScrollRange->getScrollRange() / 6.0f;
	float sector_current = (float)_position / sector_size;

	// текущий сектор
	size_t current = (size_t)sector_current;
	assert(current < 6);
	// смещение до следующего сектора от 0 до 1
	float offfset = (sector_current - (float)current);

	const Ogre::ColourValue & from = mColourRange[current];
	const Ogre::ColourValue & to = mColourRange[current + 1];

	mBaseColour.r = from.r + offfset * (to.r - from.r);
	mBaseColour.g = from.g + offfset * (to.g - from.g);
	mBaseColour.b = from.b + offfset * (to.b - from.b);

	mRawColourRect->setRectColour(Ogre::ColourValue::White, mBaseColour, Ogre::ColourValue::White, Ogre::ColourValue::White);

	MyGUI::IntPoint point(
		mImageColourPicker->getLeft() + (mImageColourPicker->getWidth() / 2),
		mImageColourPicker->getTop() + (mImageColourPicker->getHeight() / 2));

	updateFromPoint(point);
}

void ColourPanel::notifyEditTextChange(MyGUI::WidgetPtr _sender)
{
	MyGUI::EditPtr edit = static_cast<MyGUI::EditPtr>(_sender);
	size_t cursor = edit->getTextCursor();
	size_t num = MyGUI::utility::parseSizeT(edit->getCaption());
	if (num > 255) num = 255;
	edit->setCaption(MyGUI::utility::toString(num));
	if (cursor < edit->getTextLength()) edit->setTextCursor(cursor);

	Ogre::ColourValue colour(
		MyGUI::utility::parseFloat(mEditRed->getCaption()) / 255.0f,
		MyGUI::utility::parseFloat(mEditGreen->getCaption()) / 255.0f,
		MyGUI::utility::parseFloat(mEditBlue->getCaption()) / 255.0f);
	updateFromColour(colour);
}

void ColourPanel::setColour(const Ogre::ColourValue & _colour)
{
	Ogre::ColourValue colour(_colour.saturateCopy());
	mEditRed->setCaption(MyGUI::utility::toString((int)(colour.r * 255)));
	mEditGreen->setCaption(MyGUI::utility::toString((int)(colour.g * 255)));
	mEditBlue->setCaption(MyGUI::utility::toString((int)(colour.b * 255)));

	updateFromColour(colour);
}

void ColourPanel::updateFromColour(const Ogre::ColourValue & _colour)
{
	mCurrentColour = _colour;

	// высчиываем опорный цвет

}
