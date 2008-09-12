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
	mStartColour = Ogre::ColourValue::Green;

	loadLayout();

	assignWidget(mColourRect, "widget_ColourRect");
	assignWidget(mColourView, "widget_ColourView");
	assignWidget(mImageColourPicker, "image_Picker");

	mColourRect->eventMouseButtonPressed = MyGUI::newDelegate(this, &ColourPanel::notifyMouseButtonPressed);
	mColourRect->eventMouseDrag = MyGUI::newDelegate(this, &ColourPanel::notifyMouseDrag);
	mImageColourPicker->eventMouseDrag = MyGUI::newDelegate(this, &ColourPanel::notifyMouseDrag);

	MyGUI::SubWidgetInterface * main = mColourRect->_getSubWidgetMain();
	mRawColourRect = static_cast<MyGUI::RawRect*>(main);
	main = mColourView->_getSubWidgetMain();
	mRawColourView = static_cast<MyGUI::RawRect*>(main);

	first_update();

}

void ColourPanel::show()
{
	mMainWidget->show();
}

void ColourPanel::hide()
{
	mMainWidget->hide();
}

void ColourPanel::first_update()
{
	mRawColourRect->setRectColour(Ogre::ColourValue::White, mStartColour, Ogre::ColourValue::Black, Ogre::ColourValue::Black);

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

	// вычисляем цвет. Altren 09.2008
	float x = 1. * point.left / mColourRect->getWidth();
	float y = 1. * point.top / mColourRect->getHeight();
	if (x > 1) x = 1;
	else if (x < 0) x = 0;
	if (y > 1) y = 1;
	else if (y < 0) y = 0;

	mCurrentColour = (1 - y) * (mStartColour * x + Ogre::ColourValue::White * (1 - x));

	mRawColourView->setRectColour(mCurrentColour, mCurrentColour, mCurrentColour, mCurrentColour);
}

void ColourPanel::notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
{
	if (_id == MyGUI::MB_Left) notifyMouseDrag(null, _left, _top);
}
