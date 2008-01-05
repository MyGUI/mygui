/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_SCROLL_BASE_H__
#define __MYGUI_SCROLL_BASE_H__

#include "MyGUI_Widget.h"
#include "MyGUI_Button.h"

namespace MyGUI
{

	class ScrollBase;
	typedef ScrollBase * ScrollBasePtr;

	class _MyGUIExport ScrollBase : public Widget
	{

	protected:
		ScrollBase(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

	public:

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top);

		virtual void notifyTrackMove(int _left, int _top) = 0;

		void setScrollRange(size_t _range);
		inline size_t getScrollRange() {return mScrollRange;}

		void setScrollPosition(size_t _position);
		inline size_t getScrollPosition() {return mScrollPosition;}

		inline void setScrollPage(size_t _page = 1) {mScrollPage = _page;}
		inline size_t getScrollPage() {return mScrollPage;}

		virtual void updateTrack() = 0;
		virtual void setSize(const IntSize& _size);
		virtual void setPosition(const IntCoord& _coord);

		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// event : изменилось положение скрола
		// signature : void method(MyGUI::WidgetPtr _widget, int _position)
		EventSimpleDataInfo eventScrollChangePosition;

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

	}; // class ScrollBase : public Widget

} // namespace MyGUI

#endif // __MYGUI_SCROLL_BASE_H__
