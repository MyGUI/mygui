/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#include "Precompiled.h"
#include "ColourWindow.h"

namespace demo
{

	int ColourWindow::getRand(int _min, int _max)
	{
		if (_max < _min) std::swap(_max, _min);
		int range = _max - _min;
		if (range == 0) return 0;
		int result = ::rand() % range;
		if (result < 0) result = -result;
		return _min + result;
	}

	ColourWindow::ColourWindow(MyGUI::Widget* _parent) : BaseLayout("ColourWindow.layout", _parent)
	{
		assignWidget(mSliderRed, "Red");
		assignWidget(mSliderGreen, "Green");
		assignWidget(mSliderBlue, "Blue");
		assignWidget(mColour, "Colour");
		assignWidget(mAdd, "Add");
		assignWidget(mLine, "Line");
		assignBase(mBox, "Box");

		mSliderRed->eventScrollChangePosition += MyGUI::newDelegate(this, &ColourWindow::notifyScrollChangePosition);
		mSliderGreen->eventScrollChangePosition += MyGUI::newDelegate(this, &ColourWindow::notifyScrollChangePosition);
		mSliderBlue->eventScrollChangePosition += MyGUI::newDelegate(this, &ColourWindow::notifyScrollChangePosition);

		mAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &ColourWindow::notifyMouseButtonClick);
		mLine->eventEditSelectAccept += MyGUI::newDelegate(this, &ColourWindow::notifyEditSelectAccept);

		if (_parent)
		{
			const MyGUI::IntCoord& coord = _parent->getClientCoord();
			const MyGUI::IntSize& size = mMainWidget->getSize();
			mMainWidget->setPosition(MyGUI::IntPoint(getRand(0, coord.width - size.width), getRand(0, coord.height - size.height)));
		}

		// update
		notifyScrollChangePosition(nullptr, 0);
	}

	ColourWindow::~ColourWindow()
	{
		//mBox.shutdown();
	}

	void ColourWindow::notifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position)
	{
		MyGUI::Colour colour(float(mSliderRed->getScrollPosition()) / float(mSliderRed->getScrollRange()),
			float(mSliderGreen->getScrollPosition()) / float(mSliderGreen->getScrollRange()),
			float(mSliderBlue->getScrollPosition()) / float(mSliderBlue->getScrollRange()) );

		mColour->setColour(colour);
	}

	void ColourWindow::notifyEditSelectAccept(MyGUI::EditBox* _sender)
	{
		notifyMouseButtonClick(nullptr);
	}

	void ColourWindow::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		MyGUI::Colour colour(float(mSliderRed->getScrollPosition()) / float(mSliderRed->getScrollRange()),
			float(mSliderGreen->getScrollPosition()) / float(mSliderGreen->getScrollRange()),
			float(mSliderBlue->getScrollPosition()) / float(mSliderBlue->getScrollRange()) );

		mBox->addColourItem(colour, mLine->getOnlyText());
		mLine->setCaption("");
	}

} // namespace demo
