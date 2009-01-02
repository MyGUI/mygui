/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "ColourWindow.h"

namespace demo
{

	ColourWindow::ColourWindow() :
		BaseLayout("ColourWindow.layout")
	{
	}

	void ColourWindow::initialise()
	{
		loadLayout();

		assignWidget(mSliderRed, "Red");
		assignWidget(mSliderGreen, "Green");
		assignWidget(mSliderBlue, "Blue");
		assignWidget(mColour, "Colour");
		assignWidget(mAdd, "Add");
		assignWidget(mLine, "Line");
		assignWidget(mBox, "Box");

		mSliderRed->eventScrollChangePosition = MyGUI::newDelegate(this, &ColourWindow::notifyScrollChangePosition);
		mSliderGreen->eventScrollChangePosition = MyGUI::newDelegate(this, &ColourWindow::notifyScrollChangePosition);
		mSliderBlue->eventScrollChangePosition = MyGUI::newDelegate(this, &ColourWindow::notifyScrollChangePosition);

		mRawColourView = mColour->getSubWidgetMain()->castType<MyGUI::RawRect>();

		mAdd->eventMouseButtonClick = MyGUI::newDelegate(this, &ColourWindow::notifyMouseButtonClick);
		mLine->eventEditSelectAccept = MyGUI::newDelegate(this, &ColourWindow::notifyMouseButtonClick);

		// update
		notifyScrollChangePosition(null, 0);
	}

	void ColourWindow::notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _position)
	{
		MyGUI::Colour colour(float(mSliderRed->getScrollPosition()) / float(mSliderRed->getScrollRange()),
			float(mSliderGreen->getScrollPosition()) / float(mSliderGreen->getScrollRange()),
			float(mSliderBlue->getScrollPosition()) / float(mSliderBlue->getScrollRange()) );

		mRawColourView->setRectColour(colour, colour, colour, colour);
	}

	void ColourWindow::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		MyGUI::Colour colour(float(mSliderRed->getScrollPosition()) / float(mSliderRed->getScrollRange()),
			float(mSliderGreen->getScrollPosition()) / float(mSliderGreen->getScrollRange()),
			float(mSliderBlue->getScrollPosition()) / float(mSliderBlue->getScrollRange()) );

		mBox.addColourItem(colour, mLine->getCaption());
		mLine->setCaption("");
	}

} // namespace demo
