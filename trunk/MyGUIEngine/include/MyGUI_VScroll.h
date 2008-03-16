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
		VScroll(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {static Ogre::String type("VScroll"); return type;}
		//!	@copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		virtual void TrackMove(int _left, int _top);
		virtual void updateTrack();

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);

		void setScrollRange(size_t _range);
		inline size_t getScrollRange() {return mScrollRange;}

		void setScrollPosition(size_t _position);
		inline size_t getScrollPosition() {return mScrollPosition;}

		inline void setScrollPage(size_t _page = 1) {mScrollPage = _page;}
		inline size_t getScrollPage() {return mScrollPage;}

		virtual void setSize(const IntSize& _size);
		virtual void setPosition(const IntCoord& _coord);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// event : изменилось положение скрола
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _position)
		EventInfo_WidgetSizeT eventScrollChangePosition;

	protected:
		// наши кнопки
		ButtonPtr mWidgetStart;
		ButtonPtr mWidgetEnd;
		ButtonPtr mWidgetTrack;

		// размеры окна перед началом его изменений
		IntRect mPreActionRect;

		// диапазон на который трек может двигаться
		size_t mSkinRangeStart;
		size_t mSkinRangeEnd;

		size_t mScrollRange;
		size_t mScrollPosition;
		size_t mScrollPage; // на сколько перещелкивать, при щелчке на кнопке

	}; // class VScroll : public Widget

} // namespace MyGUI

#endif // __MYGUI_VSCROLL_H__
