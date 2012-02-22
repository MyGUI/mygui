/*!
	@file
	@author		George Evmenov
	@date		02/2011
*/
#include "BerkeliumDelegate.h"
#include "BerkeliumWidget.h"
#include "MyGUI_PointerManager.h"
#include <berkelium/Cursor.hpp>

namespace MyGUI
{

	BerkeliumDelegate::BerkeliumDelegate() :
		mWindow(nullptr),
		mBuffer(nullptr),
		mWidget(nullptr)
	{
	}

	void BerkeliumDelegate::setWindow(Berkelium::Window* _window)
	{
		mWindow = _window;
	}

	void BerkeliumDelegate::setBuffer(helpers::Buffer* _buffer)
	{
		mBuffer = _buffer;
	}

	void BerkeliumDelegate::setWidget(MyGUI::BerkeliumWidget* _widget)
	{
		mWidget = _widget;
	}

	void BerkeliumDelegate::onPaint(
		Berkelium::Window *win,
		const unsigned char *sourceBuffer,
		const Berkelium::Rect &sourceBufferRect,
		size_t numCopyRects,
		const Berkelium::Rect *copyRects,
		int dx, int dy,
		const Berkelium::Rect &scrollRect)
	{
		if (mWindow == nullptr || mBuffer == nullptr)
			return;

		mBuffer->scroll(dx, dy, scrollRect.left(), scrollRect.top(), scrollRect.width(), scrollRect.height());

		for (size_t i = 0; i < numCopyRects; i++)
		{
			const Berkelium::Rect &updateRect = copyRects[i];

			mBuffer->update((void*)sourceBuffer, updateRect.left(), updateRect.top(), updateRect.width(), updateRect.height(),
				updateRect.left() - sourceBufferRect.left(), updateRect.top() - sourceBufferRect.top(), sourceBufferRect.width());
		}
	}

	void BerkeliumDelegate::onCursorUpdated(Berkelium::Window *win, const Berkelium::Cursor& newCursor)
	{
		if (mWidget == nullptr)
			return;

		std::string cursor = "arrow";
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		static HCURSOR beam_cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM));
		static HCURSOR link_cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));

		if (newCursor.GetCursor() == beam_cursor)
		{
			cursor = "beam";
		}
		else if (newCursor.GetCursor() == link_cursor)
		{
			cursor = "link";
		}
#elif  MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
		// TODO
#elif  MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
		// TODO
#endif
		mWidget->setPointer(cursor);
		MyGUI::PointerManager::getInstance().setPointer(cursor);
		MyGUI::PointerManager::getInstance().eventChangeMousePointer(cursor);
	}

} // namespace MyGUI
