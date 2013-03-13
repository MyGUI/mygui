/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/

#ifndef __MYGUI_SCROLL_VIEW_PANEL_H__
#define __MYGUI_SCROLL_VIEW_PANEL_H__

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
		virtual void setPosition(const IntPoint& _value);
		//! @copydoc Widget::setSize(const IntSize& _value)
		virtual void setSize(const IntSize& _value);
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		virtual void setCoord(const IntCoord& _value);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top);
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height);
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height);

		void updateContent();
	};

} // namespace MyGUI

#endif // __MYGUI_SCROLL_VIEW_PANEL_H__
