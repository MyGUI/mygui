/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_SCROLL_BAR_H__
#define __MYGUI_SCROLL_BAR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	typedef delegates::CMultiDelegate2<ScrollBar*, size_t> EventHandle_ScrollBarPtrSizeT;

	/** \brief @wpage{ScrollBar}
		ScrollBar widget description should be here.
	*/
	class MYGUI_EXPORT ScrollBar :
		public Widget,
		public MemberObsolete<ScrollBar>
	{
		MYGUI_RTTI_DERIVED( ScrollBar )

	public:
		ScrollBar();

		/** Set vertical alignment grid mode */
		void setVerticalAlignment(bool _value);
		/** Get vertical alignment grid mode flag */
		bool getVerticalAlignment() const;

		/** Set scroll range */
		void setScrollRange(size_t _value);
		/** Get scroll range */
		size_t getScrollRange() const;

		/** Set scroll position (value from 0 to range - 1) */
		void setScrollPosition(size_t _value);
		/** Get scroll position (value from 0 to range - 1) */
		size_t getScrollPosition() const;

		/** Set scroll page
			@param _value Tracker step when buttons pressed
		*/
		void setScrollPage(size_t _value);
		/** Get scroll page */
		size_t getScrollPage() const;

		/** Set scroll view page
			@param _value Tracker step when pressed on scroll line
		*/
		void setScrollViewPage(size_t _value);
		/** Get scroll view page */
		size_t getScrollViewPage() const;

		/** Get size in pixels of area where scroll moves */
		int getLineSize() const;

		/** Set size of track in pixels
			@param _value in pixels, if less than MinTrackSize, MinTrackSize used
		*/
		void setTrackSize(int _value);
		/** Get size of track in pixels */
		int getTrackSize() const;

		/** Set minimal track size (used for setTrackSize)*/
		void setMinTrackSize(int _value);
		/** Get minimal track size */
		int getMinTrackSize() const;

		/** Enable or disable move to click mode.\n
			Move to click mode: Tracker moves to cursor when pressed on scroll line.\n
			Disabled (false) by default.
		*/
		void setMoveToClick(bool _value);
		/** Get move to click mode flag */
		bool getMoveToClick() const;

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

		/*events:*/
		/** Event : scroll tracker position changed.\n
			signature : void method(MyGUI::ScrollBar* _sender, size_t _position)\n
			@param _sender widget that called this event
			@param _position - new tracker position
		*/
		EventHandle_ScrollBarPtrSizeT eventScrollChangePosition;

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		void updateTrack();
		void TrackMove(int _left, int _top);

		virtual void onMouseWheel(int _rel);

		void notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseDrag(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseWheel(Widget* _sender, int _rel);

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

		int getTrackPlaceLength() const;

	protected:
		// наши кнопки
		Button* mWidgetStart;
		Button* mWidgetEnd;
		Button* mWidgetTrack;
		// куски между кнопками
		Widget* mWidgetFirstPart;
		Widget* mWidgetSecondPart;

		// смещение внутри окна
		IntPoint mPreActionOffset;

		// диапазон на который трек может двигаться
		size_t mSkinRangeStart;
		size_t mSkinRangeEnd;

		size_t mScrollRange;
		size_t mScrollPosition;
		size_t mScrollPage; // на сколько перещелкивать, при щелчке на кнопке
		size_t mScrollViewPage; // на сколько перещелкивать, при щелчке по полосе

		int mMinTrackSize;
		bool mMoveToClick;

		bool mVerticalAlignment;
	};

} // namespace MyGUI

#endif // __MYGUI_SCROLL_BAR_H__
