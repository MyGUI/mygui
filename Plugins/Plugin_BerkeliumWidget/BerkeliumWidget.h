/*!
	@file
	@author		Albert Semenov
	@date		02/2010
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

		MYGUI_RTTI_DERIVED( BerkeliumWidget );

	public:
		BerkeliumWidget();

		void loadURL(const std::string& _url);

	private:
		// from WindowDelegate
		virtual void onAddressBarChanged(Berkelium::Window* win, const char* newURL, size_t newURLSize) { }

		virtual void onStartLoading(Berkelium::Window* win, const char* newURL, size_t newURLSize) { }
		virtual void onLoad(Berkelium::Window* win) { }
		virtual void onLoadError(Berkelium::Window* win, const char* error, size_t errorLength) { }

		virtual void onBeforeUnload(Berkelium::Window* win, bool* proceed) { }
		virtual void onCancelUnload(Berkelium::Window* win) { }

		virtual void onCrashed(Berkelium::Window* win) { }
		virtual void onUnresponsive(Berkelium::Window* win) { }
		virtual void onResponsive(Berkelium::Window* win) { }
		virtual void onChromeSend(Berkelium::Window* win, Berkelium::WindowDelegate::Data message, const Berkelium::WindowDelegate::Data* content, size_t numContents) { }
		virtual void onCreatedWindow(Berkelium::Window* win, Berkelium::Window* newWindow) { }
		virtual void onPaint(Berkelium::Window* win, const unsigned char* sourceBuffer, const Berkelium::Rect& rect, int dx, int dy, const Berkelium::Rect& scrollRect);

		virtual void onWidgetCreated(Berkelium::Window* win, Berkelium::Widget* newWidget, int zIndex) { }
		virtual void onWidgetDestroyed(Berkelium::Window* win, Berkelium::Widget* newWidget) { }
		virtual void onWidgetResize(Berkelium::Window* win, Berkelium::Widget* wid, int newWidth, int newHeight) { }
		virtual void onWidgetMove(Berkelium::Window* win, Berkelium::Widget* wid, int newX, int newY) { }
		virtual void onWidgetPaint(Berkelium::Window* win, Berkelium::Widget* wid, const unsigned char* sourceBuffer, const Berkelium::Rect& rect, int dx, int dy, const Berkelium::Rect& scrollRect) { }

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

		virtual void onMouseDrag(int _left, int _top, MouseButton _id);
		virtual void onMouseMove(int _left, int _top);
		virtual void onMouseWheel(int _rel);
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onKeyLostFocus(Widget* _new);
		virtual void onKeySetFocus(Widget* _old);
		virtual void onKeyButtonPressed(KeyCode _key, Char _char);
		virtual void onKeyButtonReleased(KeyCode _key);

	private:
		void notifyUpdateCanvas(Canvas* _canvas, Canvas::Event _event);
		void notifyFrameStart(float _time);
		void update(bool _invalidate);
		void updateSize();

	private:
		Berkelium::Window* mWindow;
		int mOldWidth;
		int mOldHeight;
		helpers::Buffer mBuffer;
	};

} // namespace MyGUI

#endif // __BERKELIUM_WIDGET_H__
