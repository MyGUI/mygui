/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_VSCROLL_H__
#define __MYGUI_VSCROLL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class _MyGUIExport VScroll : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::VScrollFactory;

	protected:
		VScroll(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);

		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/** Set scroll range */
		void setScrollRange(size_t _range);
		/** Get scroll range */
		inline size_t getScrollRange() {return mScrollRange;}

		/** Set scroll position */
		void setScrollPosition(size_t _position);
		/** Get scroll position */
		inline size_t getScrollPosition() {return mScrollPosition;}

		/** Set scroll page
			page - tracker step when buttons pressed
		*/
		inline void setScrollPage(size_t _page = 1) {mScrollPage = _page;}
		/** Get scroll page */
		inline size_t getScrollPage() {return mScrollPage;}

		/** Set scroll view page
			view page - tracker step when pressed on scroll line
		*/
		inline void setScrollViewPage(size_t _viewPage = 1) {mScrollViewPage = _viewPage;}
		/** Get scroll view page */
		inline size_t getScrollViewPage() {return mScrollViewPage;}

		/** Get size in pixels of area where scroll moves */
		virtual int getLineSize();

		/** Set size of track in pixels
			@param _size in pixels, if less than MinTrackSize, MinTrackSize used
		*/
		virtual void setTrackSize(size_t _size);
		/** Get size of track in pixels */
		virtual int getTrackSize();

		/** Set minimal track size (used for setTrackSize)*/
		void setMinTrackSize(int _size) {mMinTrackSize = _size;}
		/** Get minimal track size */
		int getMinTrackSize() {return mMinTrackSize;}

		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		virtual void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		//! @copydoc Widget::setSize(int _width, int _height)
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		/** Event : scroll tracker position changed.\n
			signature : void method(WidgetPtr _sender, size_t _position)\n
			@param _position - new tracker position
		*/
		EventInfo_WidgetSizeT eventScrollChangePosition;

	protected:
		virtual void updateTrack();
		virtual void TrackMove(int _left, int _top);
		void notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseDrag(WidgetPtr _sender, int _left, int _top);

		// наши кнопки
		ButtonPtr mWidgetStart;
		ButtonPtr mWidgetEnd;
		ButtonPtr mWidgetTrack;
		// куски между кнопками
		ButtonPtr mWidgetFirstPart;
		ButtonPtr mWidgetSecondPart;

		// размеры окна перед началом его изменений
		IntRect mPreActionRect;

		// диапазон на который трек может двигаться
		size_t mSkinRangeStart;
		size_t mSkinRangeEnd;

		size_t mScrollRange;
		size_t mScrollPosition;
		size_t mScrollPage; // на сколько перещелкивать, при щелчке на кнопке
		size_t mScrollViewPage; // на сколько перещелкивать, при щелчке по полосе

		int mMinTrackSize;

	}; // class VScroll : public Widget

} // namespace MyGUI

#endif // __MYGUI_VSCROLL_H__
