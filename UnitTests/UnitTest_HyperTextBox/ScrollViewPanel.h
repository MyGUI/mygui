/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/

#ifndef MYGUI_SCROLL_VIEW_PANEL_H_
#define MYGUI_SCROLL_VIEW_PANEL_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ScrollView.h"

namespace MyGUI
{

	class ScrollViewPanel :
		public ScrollView
	{
		MYGUI_RTTI_DERIVED( ScrollViewPanel )

	public:
		ScrollViewPanel();

		//! @copydoc Widget::setPosition(const IntPoint& _value)
		void setPosition(const IntPoint& _value) override;
		//! @copydoc Widget::setSize(const IntSize& _value)
		void setSize(const IntSize& _value) override;
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		void setCoord(const IntCoord& _value) override;

		using Widget::setPosition;
		using Widget::setSize;
		using Widget::setCoord;

		void updateContent();
	};

} // namespace MyGUI

#endif // MYGUI_SCROLL_VIEW_PANEL_H_
