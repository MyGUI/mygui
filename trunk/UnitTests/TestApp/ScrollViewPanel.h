/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_SCROLL_VIEW_PANEL_H__
#define __MYGUI_SCROLL_VIEW_PANEL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_ScrollViewBase.h"

namespace MyGUI
{

	class ScrollViewPanel :
		public Widget,
		protected ScrollViewBase
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

		/** Show VScroll when text size larger than Edit */
		void setVisibleVScroll(bool _value);
		/** Get Show VScroll flag */
		bool isVisibleVScroll() const;

		/** Show HScroll when text size larger than Edit */
		void setVisibleHScroll(bool _value);
		/** Get Show HScroll flag */
		bool isVisibleHScroll() const;

		/** Set canvas align */
		void setCanvasAlign(Align _value);
		/** Get canvas align */
		Align getCanvasAlign() const;

		/** Set canvas size */
		void setCanvasSize(const IntSize& _value);
		/** Set canvas size */
		void setCanvasSize(int _width, int _height);
		/** Get canvas size */
		IntSize getCanvasSize();

		/** Get view area coordinates. */
		IntCoord getViewCoord();

		/** Set view area offset. */
		void setViewOffset(const IntPoint& _value);
		/** Get view area offset. */
		IntPoint getViewOffset();

		void updateContent();

		//! Get number of items
		size_t getItemCount() const;

		//! Insert an item into a array at a specified position
		//void insertItemAt(size_t _index, const UString& _item);

		//! Add an item to the end of a array
		void addItem(const UString& _item);

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();

		//! Swap items at a specified positions
		//void swapItemsAt(size_t _index1, size_t _index2);

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		//virtual void onWidgetCreated(Widget* _widget);
		//virtual void onWidgetDestroy(Widget* _widget);

		void notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(Widget* _sender, int _left, int _top, MouseButton _id);

		void notifyScrollChangePosition(ScrollBar* _sender, size_t _position);
		void notifyMouseWheel(Widget* _sender, int _rel);

		void updateView();

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		// ������ ������
		virtual IntSize getContentSize();
		// �������� ������
		virtual IntPoint getContentPosition();
		// ������ ����, ����� ������� ����� ������
		virtual IntSize getViewSize();
		virtual void setContentPosition(const IntPoint& _point);
		// ������ �� ������� �������������� ������ ��� ������ �� ������
		virtual size_t getVScrollPage();
		virtual size_t getHScrollPage();

		virtual Align getContentAlign();

		/*void parseHyperText(const UString& _value, Widget* _parent);
		void parseTag(const UString& _value, Widget* _parent);
		void parseText(const UString& _value, Widget* _parent);*/

	protected:
		Align mContentAlign;
		Widget* mRealClient;

	/*private:
		Widget* mStackPanel;*/
	};

} // namespace MyGUI

#endif // __MYGUI_SCROLL_VIEW_PANEL_H__
