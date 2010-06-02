/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __COLOUR_WINDOW_H__
#define __COLOUR_WINDOW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "ColourWindowBox.h"

namespace demo
{

	class ColourWindow : public wraps::BaseLayout
	{
	public:
		ColourWindow(MyGUI::Widget* _parent);
		virtual ~ColourWindow();

	private:
		void notifyScrollChangePosition(MyGUI::VScroll* _sender, size_t _position);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyEditSelectAccept(MyGUI::Edit* _sender);
		int getRand(int _min, int _max);

	private:
		MyGUI::HScroll* mSliderRed;
		MyGUI::HScroll* mSliderGreen;
		MyGUI::HScroll* mSliderBlue;

		MyGUI::Widget* mColour;
		MyGUI::RawRect * mRawColourView;
		MyGUI::Button* mAdd;
		MyGUI::Edit* mLine;

		ColourWindowBox * mBox;

	};

} // namespace demo

#endif // __COLOUR_WINDOW_H__
