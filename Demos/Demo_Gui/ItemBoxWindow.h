/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __ITEM_BOX_WINDOW_H__
#define __ITEM_BOX_WINDOW_H__

#include <MyGUI.h>
#include "BaseLayout.h"

namespace demo
{

	class ItemBoxWindow : public wraps::BaseLayout
	{
	public:
		ItemBoxWindow();

		virtual void initialise();

	private:
		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _position);
		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);

	private:
		MyGUI::HScrollPtr mSliderRed;
		MyGUI::HScrollPtr mSliderGreen;
		MyGUI::HScrollPtr mSliderBlue;

		MyGUI::WidgetPtr mColour;
		MyGUI::RawRect * mRawColourView;
		MyGUI::ButtonPtr mAdd;
		MyGUI::EditPtr mLine;
		

	};

} // namespace demo

#endif // __ITEM_BOX_WINDOW_H__
