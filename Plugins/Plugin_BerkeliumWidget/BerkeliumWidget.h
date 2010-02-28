/*!
	@file
	@author		Albert Semenov
	@date		02/2010
	@module
*/
#ifndef __BERKELIUM_WIDGET_H__
#define __BERKELIUM_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Canvas.h"
#include "Buffer.h"
#include <berkelium/Berkelium.hpp>
#include <berkelium/Window.hpp>
#include <berkelium/WindowDelegate.hpp>

namespace MyGUI
{

	class BerkeliumWidget :
		public Canvas,
		public Berkelium::WindowDelegate
	{
		// для вызова закрытого конструктора
		friend class BerkeliumWidgetFactory;

		MYGUI_RTTI_CHILD_HEADER( BerkeliumWidget, Canvas );

	public:
		BerkeliumWidget();

		void loadURL(const std::string& _url);

		virtual void setProperty(const std::string& _key, const std::string& _value);

	/*internal:*/
		virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name);

	private:
		// from WindowDelegate
		virtual void onAddressBarChanged(Berkelium::Window *win, const char* newURL, size_t newURLSize) { }

		virtual void onStartLoading(Berkelium::Window *win, const char *newURL, size_t newURLSize) { }
		virtual void onLoad(Berkelium::Window *win) { }
		virtual void onLoadError(Berkelium::Window *win, const char* error, size_t errorLength) { }

		virtual void onBeforeUnload(Berkelium::Window *win, bool *proceed) { }
		virtual void onCancelUnload(Berkelium::Window *win) { }

		virtual void onCrashed(Berkelium::Window *win) { }
		virtual void onUnresponsive(Berkelium::Window *win) { }
		virtual void onResponsive(Berkelium::Window *win) { }
		virtual void onChromeSend(Berkelium::Window *win, Berkelium::WindowDelegate::Data message, const Berkelium::WindowDelegate::Data* content, size_t numContents) { }
		virtual void onCreatedWindow(Berkelium::Window *win, Berkelium::Window *newWindow) { }
		virtual void onPaint(Berkelium::Window *win, const unsigned char *sourceBuffer, const Berkelium::Rect &rect, int dx, int dy, const Berkelium::Rect &scrollRect);

		virtual void onWidgetCreated(Berkelium::Window *win, Berkelium::Widget *newWidget, int zIndex) { }
	    virtual void onWidgetDestroyed(Berkelium::Window *win, Berkelium::Widget *newWidget) { }
		virtual void onWidgetResize(Berkelium::Window *win, Berkelium::Widget *wid, int newWidth, int newHeight) { }
		virtual void onWidgetMove(Berkelium::Window *win, Berkelium::Widget *wid, int newX, int newY) { }
		virtual void onWidgetPaint(Berkelium::Window *win, Berkelium::Widget *wid, const unsigned char *sourceBuffer, const Berkelium::Rect &rect, int dx, int dy, const Berkelium::Rect &scrollRect) { }

	protected:
		virtual ~BerkeliumWidget();

		virtual void baseChangeWidgetSkin(ResourceSkin* _info);

	private:
		void initialiseWidgetSkin(ResourceSkin* _info);
		void shutdownWidgetSkin();

		void notifyUpdateCanvas(Canvas* _canvas, Canvas::Event _event);
		void notifyFrameStart(float _time);
		void update(bool _invalidate);
		void updateSize();


	private:
		Berkelium::Window* mWindow;
		int mOldWidth;
		int mOldHeight;
		helper::Buffer mBuffer;
	};

}

#endif // __BERKELIUM_WIDGET_H__
