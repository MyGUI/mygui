/*!
	@file
	@author		George Evmenov
	@date		02/2011
*/
#ifndef __WINDOW_DELEGATE_H__
#define __WINDOW_DELEGATE_H__

#include "MyGUI_Prerequest.h"
#include <string>
#include "Buffer.h"
#include <berkelium/WindowDelegate.hpp>

namespace MyGUI
{

	class BerkeliumWidget;

	class MYGUI_EXPORT_DLL BerkeliumDelegate :
		public Berkelium::WindowDelegate
	{
	public:
		BerkeliumDelegate();
		virtual ~BerkeliumDelegate() { }

		void setWindow(Berkelium::Window* _window);
		void setBuffer(helpers::Buffer* _buffer);
		void setWidget(MyGUI::BerkeliumWidget* _widget);

	protected:
		virtual void onPaint(
			Berkelium::Window *win,
			const unsigned char *sourceBuffer,
			const Berkelium::Rect &sourceBufferRect,
			size_t numCopyRects,
			const Berkelium::Rect *copyRects,
			int dx, int dy,
			const Berkelium::Rect &scrollRect);

		virtual void onCursorUpdated(Berkelium::Window *win, const Berkelium::Cursor& newCursor);

	private:
		Berkelium::Window* mWindow;
		helpers::Buffer* mBuffer;
		MyGUI::BerkeliumWidget* mWidget;
	};

} // namespace MyGUI

#endif // __WINDOW_DELEGATE_H__
