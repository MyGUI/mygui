/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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
#ifndef __MYGUI_SCROLL_VIEW_H__
#define __MYGUI_SCROLL_VIEW_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_ScrollViewBase.h"

namespace MyGUI
{

	class MYGUI_EXPORT ScrollView :
		public Widget,
		protected ScrollViewBase,
		public MemberObsolete<ScrollView>
	{
		MYGUI_RTTI_DERIVED( ScrollView )

	public:
		ScrollView();

		//! @copydoc Widget::setPosition(const IntPoint& _value)
		virtual void setPosition(const IntPoint& _value);
		//! @copydoc Widget::setSize(const IntSize& _value)
		virtual void setSize(const IntSize& _value);
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		virtual void setCoord(const IntCoord& _value);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		/** Show VScroll when text size larger than Edit */
		void setVisibleVScroll(bool _value);
		/** Get Show VScroll flag */
		bool isVisibleVScroll() { return mVisibleVScroll; }

		/** Show HScroll when text size larger than Edit */
		void setVisibleHScroll(bool _value);
		/** Get Show HScroll flag */
		bool isVisibleHScroll() { return mVisibleHScroll; }

		/** Set canvas align */
		void setCanvasAlign(Align _value);
		/** Get canvas align */
		Align getCanvasAlign() { return mContentAlign; }

		/** Set canvas size */
		void setCanvasSize(const IntSize& _value);
		/** Set canvas size */
		void setCanvasSize(int _width, int _height) { setCanvasSize(IntSize(_width, _height)); }
		/** Get canvas size */
		IntSize getCanvasSize();

		/** Get rect where child widgets placed */
		const IntCoord& getClientCoord();

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		// переопределяем для присвоению холста
		virtual Widget* baseCreateWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name);

		void notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(Widget* _sender, int _left, int _top, MouseButton _id);

		void notifyScrollChangePosition(VScroll* _sender, size_t _position);
		void notifyMouseWheel(Widget* _sender, int _rel);

		void updateView();

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		// размер данных
		virtual IntSize getContentSize();
		// смещение данных
		virtual IntPoint getContentPosition();
		virtual void setContentPosition(const IntPoint& _point);
		// размер окна, через которые видно данные
		virtual IntSize getViewSize() const;
		// размер на который прокручиваются данные при щелчке по скролу
		virtual size_t getVScrollPage();
		virtual size_t getHScrollPage();

		virtual Align getContentAlign() { return mContentAlign; }

	protected:
		Widget* mScrollClient;
		Align mContentAlign;

	};

} // namespace MyGUI

#endif // __MYGUI_SCROLL_VIEW_H__
