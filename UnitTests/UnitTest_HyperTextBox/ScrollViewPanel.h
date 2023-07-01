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

	class ScrollViewPanel : public ScrollView
	{
		MYGUI_RTTI_DERIVED(ScrollViewPanel)

	public:
		//! @copydoc Widget::setPosition(const IntPoint& _point)
		void setPosition(const IntPoint& _point) override;
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size) override;
		//! @copydoc Widget::setCoord(const IntCoord& _coord)
		void setCoord(const IntCoord& _coord) override;

		using Widget::setPosition;
		using Widget::setSize;
		using Widget::setCoord;

		void updateContent();
	};

} // namespace MyGUI

#endif // MYGUI_SCROLL_VIEW_PANEL_H_
