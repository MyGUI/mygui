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

	class ColourWindow :
		public wraps::BaseLayout
	{
	public:
		ColourWindow(MyGUI::Widget* _parent);
		virtual ~ColourWindow();

	private:
		void notifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyEditSelectAccept(MyGUI::EditBox* _sender);
		int getRand(int _min, int _max);

	private:
		MyGUI::ScrollBar* mSliderRed;
		MyGUI::ScrollBar* mSliderGreen;
		MyGUI::ScrollBar* mSliderBlue;

		MyGUI::Widget* mColour;
		MyGUI::Button* mAdd;
		MyGUI::EditBox* mLine;

		ColourWindowBox* mBox;
	};

} // namespace demo

#endif // __COLOUR_WINDOW_H__
