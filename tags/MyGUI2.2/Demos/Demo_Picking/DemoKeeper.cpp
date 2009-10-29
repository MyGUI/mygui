/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "WebCore.h"
#include "KeyboardHook.h"

namespace demo
{
	Awesomium::WebCore* core = 0; 
	Awesomium::WebView* view = 0;
	MyGUI::CanvasPtr gCanvas = 0;
	int old_width = 0;
	int old_height = 0;
	bool isTransparent = false;

	struct HookListenerImpl : public HookListener
	{
		virtual void handleKeyMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (view && MyGUI::InputManager::getInstance().getKeyFocusWidget() == gCanvas)
			{
				view->injectKeyboardEvent(hwnd, msg, wParam, lParam);
			}
		}
	};

	struct WebViewListenerImpl : public Awesomium::WebViewListener
	{
		virtual void onBeginNavigation(const std::string& url, const std::wstring& frameName) { }
		virtual void onBeginLoading(const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType) { }
		virtual void onFinishLoading() { }
		virtual void onCallback(const std::string& name, const Awesomium::JSArguments& args) { }
		virtual void onReceiveTitle(const std::wstring& title, const std::wstring& frameName) { }
		virtual void onChangeTooltip(const std::wstring& tooltip) { }

#if defined(_WIN32)
		virtual void onChangeCursor(const HCURSOR& cursor)
		{
			static HCURSOR arrow_cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
			static HCURSOR beam_cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM));
			static HCURSOR link_cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));
			static HCURSOR old_cursor = arrow_cursor;

			if (old_cursor != cursor)
			{
				old_cursor = cursor;

				if (cursor == beam_cursor)
				{
					gCanvas->setPointer("beam");
					MyGUI::PointerManager::getInstance().setPointer("beam", gCanvas);
				}
				else if (cursor == link_cursor)
				{
					gCanvas->setPointer("link");
					MyGUI::PointerManager::getInstance().setPointer("link", gCanvas);
				}
				else
				{
					gCanvas->setPointer("arrow");
					MyGUI::PointerManager::getInstance().setPointer("arrow", gCanvas);
				}
			}
		}
#endif

		virtual void onChangeKeyboardFocus(bool isFocused)
		{
			if (isFocused)
			{
				//MyGUI::MYGUI_OUT("focus");
				MyGUI::InputManager::getInstance().setKeyFocusWidget(gCanvas);
			}
			else
			{
				//MyGUI::MYGUI_OUT("unfocus");
				/*if (MyGUI::InputManager::getInstance().getKeyFocusWidget() == gCanvas)
				{
					MyGUI::InputManager::getInstance().setKeyFocusWidget(0);
				}*/
			}
		}
		virtual void onChangeTargetURL(const std::string& url) { }
	};

	WebViewListenerImpl gWebViewListenerImpl;

	HookListenerImpl gHookListenerImpl;
	KeyboardHook gKeyboardHook(&gHookListenerImpl);

	void setupAwesomium(int _width, int _height)
	{
		core = new Awesomium::WebCore(Awesomium::LOG_VERBOSE);

		view = core->createWebView(_width, _height, isTransparent, true, 90);

		view->setListener(&gWebViewListenerImpl);

		//core->setBaseDirectory("D:\\_awesomium_v1.0_demo_src_win32\\media\\");
		view->loadURL("http://google.com");
	}

	void draw(bool _update = false)
	{
		if (!view->isDirty() && !_update)
			return;

		void* data = gCanvas->lock();
		view->render((Ogre::uint8*)data, gCanvas->getTextureRealWidth() * 4, 4, 0);

		if (!isTransparent)
		{
			int count = gCanvas->getTextureRealWidth() * gCanvas->getTextureRealHeight();
			for (int index=0; index<count; ++index)
			{
				((unsigned char*)(&(((unsigned int*)data)[index])))[3] = 0xFF;
			}
		}

		gCanvas->unlock();
	}

	void requestUpdateCanvas(MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _event)
	{
		if (_event.widgetResized)
		{
			if (old_width != _canvas->getWidth() || old_height != _canvas->getHeight())
			{
				old_width = _canvas->getWidth();
				old_height = _canvas->getHeight();
				view->resize(_canvas->getWidth(), _canvas->getHeight());

				draw(true);
			}
			else if (_event.requested || _event.textureChanged)
			{
				draw(true);
			}
		}
		else if (_event.requested || _event.textureChanged)
		{
			draw(true);
		}
	}

	void eventMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		view->injectMouseMove(_left - gCanvas->getAbsoluteLeft(), _top - gCanvas->getAbsoluteTop());
	}

	void eventMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		view->injectMouseMove(_left - gCanvas->getAbsoluteLeft(), _top - gCanvas->getAbsoluteTop());
	}

	void eventMouseWheel(MyGUI::WidgetPtr _sender, int _rel)
	{
		view->injectMouseWheel(_rel < 0 ? -5 : 5);
	}

	void eventMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		view->injectMouseDown(Awesomium::MouseButton(_id.toValue()));
	}

	void eventMouseButtonReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		view->injectMouseUp(Awesomium::MouseButton(_id.toValue()));
	}

	void eventKeySetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		view->focus();
	}

	void eventKeyLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		view->unfocus();
	}

	void DemoKeeper::createScene()
	{
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Picking");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		const int width = 800;
		const int height = 600;

		MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(0, 0, width, height), MyGUI::Align::Default, "Overlapped");
		gCanvas = window->createWidget<MyGUI::Canvas>("Canvas", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);
		gCanvas->createTexture(MyGUI::Canvas::TRM_PT_VIEW_REQUESTED);
		gCanvas->requestUpdateCanvas = MyGUI::newDelegate(requestUpdateCanvas);

		old_width = gCanvas->getWidth();
		old_height = gCanvas->getHeight();

		setupAwesomium(gCanvas->getWidth(), gCanvas->getHeight());

		gCanvas->eventMouseMove = MyGUI::newDelegate(eventMouseMove);
		gCanvas->eventMouseDrag = MyGUI::newDelegate(eventMouseDrag);
		gCanvas->eventMouseWheel = MyGUI::newDelegate(eventMouseWheel);
		gCanvas->eventMouseButtonPressed = MyGUI::newDelegate(eventMouseButtonPressed);
		gCanvas->eventMouseButtonReleased = MyGUI::newDelegate(eventMouseButtonReleased);
		gCanvas->eventKeySetFocus = MyGUI::newDelegate(eventKeySetFocus);
		gCanvas->eventKeyLostFocus = MyGUI::newDelegate(eventKeyLostFocus);

		gCanvas->setNeedKeyFocus(true);

		// очищаем
		void* data = gCanvas->lock();
		memset(data, 0, gCanvas->getTextureRealWidth() * gCanvas->getTextureRealHeight() * 4);
		gCanvas->unlock();
	}

	void DemoKeeper::destroyScene()
	{
	}

	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{
		if (core)
		{
			draw();
			core->update();
		}
		return base::BaseManager::frameStarted(evt);
	}

} // namespace demo
