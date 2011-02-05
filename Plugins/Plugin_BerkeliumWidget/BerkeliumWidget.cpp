/*!
	@file
	@author		Albert Semenov
	@date		02/2010
*/
#include "BerkeliumWidget.h"
#include "../../Common/Input/InputConverter.h"
#include <berkelium/Berkelium.hpp>
#include <berkelium/Context.hpp>

namespace MyGUI
{

	BerkeliumWidget::BerkeliumWidget() :
		mWindow(nullptr),
		mOldWidth(0),
		mOldHeight(0)
	{
	}

	void BerkeliumWidget::initialiseOverride()
	{
		Base::initialiseOverride();

		// FIXME нам нужен фокус клавы
		setNeedKeyFocus(true);

		updateSize();

		createTexture(mOldWidth, mOldHeight, TRM_PT_VIEW_REQUESTED);
		requestUpdateCanvas = newDelegate(this, &BerkeliumWidget::notifyUpdateCanvas);

		Berkelium::Context *context = Berkelium::Context::create();
		mWindow = Berkelium::Window::create(context);

		updateSize();

		mBuffer.resize(mOldWidth, mOldHeight);
		mWindow->resize(mOldWidth, mOldHeight);
		mWindow->setDelegate(this);

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &BerkeliumWidget::notifyFrameStart);
	}

	void BerkeliumWidget::shutdownOverride()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &BerkeliumWidget::notifyFrameStart);

		destroyTexture();

		if (mWindow != nullptr)
		{
			mWindow->destroy();
			mWindow = nullptr;
		}
		requestUpdateCanvas = nullptr;

		Base::shutdownOverride();
	}

	void BerkeliumWidget::updateSize()
	{
		mOldWidth = std::max(1, getWidth());
		mOldHeight = std::max(1, getHeight());
	}

	////////////////////

	void BerkeliumWidget::onAddressBarChanged(Berkelium::Window *win, Berkelium::URLString newURL) {}
	void BerkeliumWidget::onStartLoading(Berkelium::Window *win, Berkelium::URLString newURL) {}
	void BerkeliumWidget::onLoad(Berkelium::Window *win) {}
	void BerkeliumWidget::onCrashedWorker(Berkelium::Window *win) {}
	void BerkeliumWidget::onCrashedPlugin(Berkelium::Window *win, Berkelium::WideString pluginName) {}
	void BerkeliumWidget::onProvisionalLoadError(Berkelium::Window *win,Berkelium:: URLString url,
												 int errorCode, bool isMainFrame) {}
	void BerkeliumWidget::onConsoleMessage(Berkelium::Window *win, Berkelium::WideString message,
										   Berkelium::WideString sourceId, int line_no) {}
	void BerkeliumWidget::onScriptAlert(Berkelium::Window *win, Berkelium::WideString message,
										Berkelium::WideString defaultValue, Berkelium::URLString url,
										int flags, bool &success, Berkelium::WideString &value) {}
	void BerkeliumWidget::freeLastScriptAlert(Berkelium::WideString lastValue) {}
	void BerkeliumWidget::onNavigationRequested(Berkelium::Window *win, Berkelium::URLString newUrl,
												Berkelium::URLString referrer, bool isNewWindow,
												bool &cancelDefaultAction) {}
	void BerkeliumWidget::onLoadingStateChanged(Berkelium::Window *win, bool isLoading) {}
	void BerkeliumWidget::onTitleChanged(Berkelium::Window *win, Berkelium::WideString title) {}
	void BerkeliumWidget::onTooltipChanged(Berkelium::Window *win, Berkelium::WideString text) {}
	void BerkeliumWidget::onCrashed(Berkelium::Window *win) {}
	void BerkeliumWidget::onUnresponsive(Berkelium::Window *win) {}
	void BerkeliumWidget::onResponsive(Berkelium::Window *win) {}
	void BerkeliumWidget::onExternalHost(
										 Berkelium::Window *win,
										 Berkelium::WideString message,
										 Berkelium::URLString origin,
										 Berkelium::URLString target) {}
	void BerkeliumWidget::onCreatedWindow(Berkelium::Window *win, Berkelium::Window *newWindow,
										  const Berkelium::Rect &initialRect) {}
	void BerkeliumWidget::onPaint(
								  Berkelium::Window *win,
								  const unsigned char *sourceBuffer,
								  const Berkelium::Rect &sourceBufferRect,
								  size_t numCopyRects,
								  const Berkelium::Rect *copyRects,
								  int dx, int dy,
								  const Berkelium::Rect &scrollRect)
	{
		if (mWindow == nullptr)	return;

		mBuffer.scroll(dx, dy, scrollRect.left(), scrollRect.top(), scrollRect.width(), scrollRect.height());

		for(size_t i = 0; i < numCopyRects; i++)
		{

			const Berkelium::Rect &updateRect = copyRects[i];

			mBuffer.update((void*)sourceBuffer, updateRect.left(), updateRect.top(), updateRect.width(), updateRect.height(),
				updateRect.left() - sourceBufferRect.left(), updateRect.top() - sourceBufferRect.top(), sourceBufferRect.width());
		}	
	}

	void BerkeliumWidget::onWidgetCreated(Berkelium::Window *win, Berkelium::Widget *newWidget, int zIndex) {}
	void BerkeliumWidget::onWidgetDestroyed(Berkelium::Window *win, Berkelium::Widget *wid) {}
	void BerkeliumWidget::onWidgetResize(
										 Berkelium::Window *win,
										 Berkelium::Widget *wid,
										 int newWidth,
										 int newHeight) {}
	void BerkeliumWidget::onWidgetMove(
									   Berkelium:: Window *win,
									   Berkelium::Widget *wid,
									   int newX,
									   int newY) {}
	void BerkeliumWidget::onWidgetPaint(
										Berkelium::Window *win,
										Berkelium::Widget *wid,
										const unsigned char *sourceBuffer,
										const Berkelium::Rect &sourceBufferRect,
										size_t numCopyRects,
										const Berkelium::Rect *copyRects,
										int dx, int dy,
										const Berkelium::Rect &scrollRect) {}
	void BerkeliumWidget::onCursorUpdated(Berkelium::Window *win, const Berkelium::Cursor& newCursor)
	{
		int a = 0;
	}
	void BerkeliumWidget::onShowContextMenu(Berkelium::Window *win,
											const Berkelium::ContextMenuEventArgs& args) {}


	///////////////////////

	void BerkeliumWidget::notifyUpdateCanvas(Canvas* _canvas, Canvas::Event _event)
	{
		if (mWindow == nullptr)
			return;

		if (_event.widgetResized)
		{
			if (mOldWidth != getWidth() || mOldHeight != getHeight())
			{
				updateSize();

				mBuffer.resize(mOldWidth, mOldHeight);
				mWindow->resize(mOldWidth, mOldHeight);

				update();
			}
			else if (_event.requested || _event.textureChanged)
			{
				update();
			}
		}
		else if (_event.requested || _event.textureChanged)
		{
			update();
		}
	}

	void BerkeliumWidget::loadURL(const std::string& _url)
	{
		if (mWindow != nullptr)
			mWindow->navigateTo(_url.data(), _url.length());
	}

	void BerkeliumWidget::notifyFrameStart(float _time)
	{
		if (mWindow == nullptr)
			return;

		if (mBuffer.isDirty())
			updateTexture();
	}

	void BerkeliumWidget::update()
	{
		void* data = lock();

		mBuffer.draw(data, getTextureRealWidth());

		unlock();
	}

	void BerkeliumWidget::onMouseDrag(int _left, int _top, MouseButton _id)
	{
		if (mWindow != nullptr)
			mWindow->mouseMoved(_left - getAbsoluteLeft(), _top - getAbsoluteTop());

		Base::onMouseDrag(_left, _top, _id);
	}

	void BerkeliumWidget::onMouseMove(int _left, int _top)
	{
		if (mWindow != nullptr)
			mWindow->mouseMoved(_left - getAbsoluteLeft(), _top - getAbsoluteTop());

		Base::onMouseMove(_left, _top);
	}

	void BerkeliumWidget::onMouseWheel(int _rel)
	{
		if (mWindow != nullptr)
			mWindow->mouseWheel(0, _rel);

		Base::onMouseWheel(_rel);
	}

	void BerkeliumWidget::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		if (mWindow != nullptr)
			mWindow->mouseButton(_id.toValue(), true);

		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void BerkeliumWidget::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		if (mWindow != nullptr)
			mWindow->mouseButton(_id.toValue(), false);

		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void BerkeliumWidget::onKeyLostFocus(Widget* _new)
	{
		if (mWindow != nullptr)
			mWindow->unfocus();

		Base::onKeyLostFocus(_new);
	}

	void BerkeliumWidget::onKeySetFocus(Widget* _old)
	{
		if (mWindow != nullptr)
			mWindow->focus();

		Base::onKeySetFocus(_old);
	}

	void BerkeliumWidget::onKeyButtonPressed(KeyCode _key, Char _char)
	{
		if (mWindow != nullptr)
		{
			InputManager& manager = InputManager::getInstance();
			int mod = 0;
			if (manager.isControlPressed())
				mod |= Berkelium::CONTROL_MOD;
			if (manager.isShiftPressed())
				mod |= Berkelium::SHIFT_MOD;

			mWindow->keyEvent(true, mod, input::ScanCodeToVirtualKey(_key.toValue()), 0);

			if (_char != 0)
			{
				wchar_t text = (wchar_t)_char;
				mWindow->textEvent(&text, 1);
			}
		}

		Base::onKeyButtonPressed(_key, _char);
	}

	void BerkeliumWidget::onKeyButtonReleased(KeyCode _key)
	{
		if (mWindow != nullptr)
		{
			InputManager& manager = InputManager::getInstance();
			int mod = 0;
			if (manager.isControlPressed())
				mod |= Berkelium::CONTROL_MOD;
			if (manager.isShiftPressed())
				mod |= Berkelium::SHIFT_MOD;

			mWindow->keyEvent(false, mod, input::ScanCodeToVirtualKey(_key.toValue()), 0);
		}

		Base::onKeyButtonReleased(_key);
	}

	void BerkeliumWidget::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		if (_key == "SourceURL")
			loadURL(_value);
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

}
