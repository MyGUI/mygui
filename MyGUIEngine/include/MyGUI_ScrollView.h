/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SCROLL_VIEW_H_
#define MYGUI_SCROLL_VIEW_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_ScrollViewBase.h"

namespace MyGUI
{

	/** \brief @wpage{ScrollView}
		ScrollView widget description should be here.
	*/
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
		void setPosition(int _left, int _top);
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height);
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height);

		/** Show VScroll when content size larger than view */
		void setVisibleVScroll(bool _value);
		/** Get Show VScroll flag */
		bool isVisibleVScroll() const;

		/** Show HScroll when content size larger than view */
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
		IntCoord getViewCoord() const;

		/** Set view area offset. */
		void setViewOffset(const IntPoint& _value);
		/** Get view area offset. */
		IntPoint getViewOffset() const;

		//Added Functions
		/** Get canvas position */
		IntPoint getCanvasPosition();
		/** Set canvas position */
		void setCanvasPosition(const IntPoint& _point);

		void setFavorVertical(bool value) { mFavorVertical = value; }
		bool getFavorVertical() { return mFavorVertical; }

		void setAllowMouseScroll(bool value) { mAllowMouseScroll = value; }
		bool getAllowMouseScroll() { return mAllowMouseScroll; }

		virtual bool onSendScrollGesture(const int& absx, const int& absy, const int& deltax, const int& deltay);

		/*events*/
		EventHandle_WidgetIntInt eventCanvasPositionChanged;

		virtual void onCanvasPositionChanged(int x, int y)
		{
			eventCanvasPositionChanged(this, x, y);// (mWidgetEventSender, x, y);
		}

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		void notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(Widget* _sender, int _left, int _top, MouseButton _id);

		void notifyScrollChangePosition(ScrollBar* _sender, size_t _position);
		void notifyMouseWheel(Widget* _sender, int _rel);

		void updateView();

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

		ScrollBar* getVScroll();

		//Added Properties
		bool mFavorVertical;
		bool mAllowMouseScroll;

		int mScrollViewMouseWheel; // колличество пикселей для колеса мыши
		int mScrollViewScrollPage; // колличество пикселей для кнопок скрола

	private:
		// размер данных
		virtual IntSize getContentSize();
		// смещение данных
		virtual IntPoint getContentPosition();
		// размер окна, через которые видно данные
		virtual IntSize getViewSize();
		virtual void setContentPosition(const IntPoint& _point);
		// размер на который прокручиваются данные при щелчке по скролу
		virtual size_t getVScrollPage();
		virtual size_t getHScrollPage();

		virtual Align getContentAlign();

	protected:
		Align mContentAlign;
		Widget* mRealClient;
	};

} // namespace MyGUI

#endif // MYGUI_SCROLL_VIEW_H_
