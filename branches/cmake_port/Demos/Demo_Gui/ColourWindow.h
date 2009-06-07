/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
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
		ColourWindow(MyGUI::WidgetPtr _parent);
		virtual ~ColourWindow();

	private:
		void notifyScrollChangePosition(MyGUI::VScrollPtr _sender, size_t _position);
		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);
		void notifyEditSelectAccept(MyGUI::EditPtr _sender);
		int getRand(int _min, int _max);

	private:
		MyGUI::HScrollPtr mSliderRed;
		MyGUI::HScrollPtr mSliderGreen;
		MyGUI::HScrollPtr mSliderBlue;

		MyGUI::WidgetPtr mColour;
		MyGUI::RawRect * mRawColourView;
		MyGUI::ButtonPtr mAdd;
		MyGUI::EditPtr mLine;

		ColourWindowBox * mBox;

	};

} // namespace demo

#endif // __COLOUR_WINDOW_H__
