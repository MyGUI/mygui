/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __MYGUI_SCROLL_VIEW_H__
#define __MYGUI_SCROLL_VIEW_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class MYGUI_EXPORT ScrollView : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<ScrollView>;

		MYGUI_RTTI_CHILD_HEADER( ScrollView, Widget );

	public:
		//! @copydoc Widget::setTextAlign
		virtual void setTextAlign(Align _align);

		//! @copydoc Widget::setPosition(const IntPoint & _point)
		virtual void setPosition(const IntPoint & _point);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		/** Show VScroll when text size larger than Edit */
		void setVisibleVScroll(bool _visible) { mShowVScroll = _visible; updateView(); }
		/** Get Show VScroll flag */
		bool isVisibleVScroll() { return mShowVScroll; }

		/** Show HScroll when text size larger than Edit */
		void setVisibleHScroll(bool _visible) { mShowHScroll = _visible; updateView(); }
		/** Get Show HScroll flag */
		bool isVisibleHScroll() { return mShowHScroll; }

		/** Get canvas align */
		Align getCanvasAlign() { return mAlignCanvas; }
		/** Set canvas align */
		void setCanvasAlign(Align _align) { mAlignCanvas = _align; updateView(); }

		/** Get canvas size */
		IntSize getCanvasSize() { return mWidgetClient->getSize(); }
		/** Set canvas size */
		void setCanvasSize(const IntSize & _size) { mWidgetClient->setSize(_size); updateView(); }
		/** Set canvas size */
		void setCanvasSize(int _width, int _height) { setCanvasSize(IntSize(_width, _height)); }

		/** Get rect where child widgets placed */
		const IntCoord& getClientCoord() { return mClient->getCoord(); }


	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord & _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }

		MYGUI_OBSOLETE("use : void ScrollView::setVisibleVScroll(bool _visible)")
		void showVScroll(bool _visible) { setVisibleVScroll(_visible); }
		MYGUI_OBSOLETE("use : bool ScrollView::isVisibleVScroll()")
		bool isShowVScroll() { return isVisibleVScroll(); }
		MYGUI_OBSOLETE("use : void ScrollView::setVisibleHScroll(bool _visible)")
		void showHScroll(bool _visible) { setVisibleHScroll(_visible); }
		MYGUI_OBSOLETE("use : bool ScrollView::isVisibleHScroll()")
		bool isShowHScroll() { return isVisibleHScroll(); }

#endif // MYGUI_DONT_USE_OBSOLETE

	protected:
		ScrollView(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~ScrollView();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		// переопределяем для присвоению холста
		virtual WidgetPtr baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);
		void notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id);

		void notifyScrollChangePosition(VScrollPtr _sender, size_t _position);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);

		virtual void onKeyLostFocus(WidgetPtr _new);
		virtual void onKeySetFocus(WidgetPtr _old);

		void updateScrollViewState();

		// обновление представления
		void updateView();

		void updateScroll();

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

	protected:
		bool mIsFocus;
		bool mIsPressed;

		VScrollPtr mVScroll;
		HScrollPtr mHScroll;

		bool mShowHScroll;
		bool mShowVScroll;

		size_t mVRange;
		size_t mHRange;

		WidgetPtr mClient;
		Align mAlignCanvas;

	}; // class ScrollView : public Widget

} // namespace MyGUI

#endif // __MYGUI_SCROLL_VIEW_H__
