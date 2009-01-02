/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/

#include "ItemBoxWindow.h"

namespace demo
{

	ItemBoxWindow::ItemBoxWindow() :
		BaseLayout("ItemBoxWindow.layout")
	{
	}

	void ItemBoxWindow::initialise()
	{
		loadLayout();

		assignWidget(mSliderRed, "Red");
		assignWidget(mSliderGreen, "Green");
		assignWidget(mSliderBlue, "Blue");
		assignWidget(mColour, "Colour");
		assignWidget(mAdd, "Add");
		assignWidget(mLine, "Line");

		mSliderRed->eventScrollChangePosition = MyGUI::newDelegate(this, &ItemBoxWindow::notifyScrollChangePosition);
		mSliderGreen->eventScrollChangePosition = MyGUI::newDelegate(this, &ItemBoxWindow::notifyScrollChangePosition);
		mSliderBlue->eventScrollChangePosition = MyGUI::newDelegate(this, &ItemBoxWindow::notifyScrollChangePosition);

		mRawColourView = mColour->getSubWidgetMain()->castType<MyGUI::RawRect>();

		mAdd->eventMouseButtonClick = MyGUI::newDelegate(this, &ItemBoxWindow::notifyMouseButtonClick);

		// update
		notifyScrollChangePosition(null, 0);
	}

	void ItemBoxWindow::notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _position)
	{
		MyGUI::Colour colour(float(mSliderRed->getScrollPosition()) / float(mSliderRed->getScrollRange()),
			float(mSliderGreen->getScrollPosition()) / float(mSliderGreen->getScrollRange()),
			float(mSliderBlue->getScrollPosition()) / float(mSliderBlue->getScrollRange()) );

		mRawColourView->setRectColour(colour, colour, colour, colour);
	}

	void ItemBoxWindow::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		std::string line = _sender->getCaption();
	}

} // namespace demo
