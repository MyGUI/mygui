/*!
	@file
	@author		Albert Semenov
	@date		10/2009
*/
#ifndef __AWESOMIUM_WIDGET_H__
#define __AWESOMIUM_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Canvas.h"
#include "MyGUI_IWidgetFactory.h"
#include "WebCore.h"
#include "WebView.h"
#include "AwesomiumWidgetFactory.h"

namespace Awesomium
{

	class AwesomiumWidget :
		public MyGUI::Canvas,
		public Awesomium::WebViewListener
	{
		// для вызова закрытого конструктора
		friend class AwesomiumWidgetFactory;

		MYGUI_RTTI_DERIVED( AwesomiumWidget );

	public:
		AwesomiumWidget();

		Awesomium::WebView* getControl() { return mControl; }

		void loadURL(const std::string& _url);

		void setTransparent(bool _value);

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		void notifyUpdateCanvas(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event);
		void notifyFrameStart(float _time);

		void update(bool _update = false);

		virtual void onMouseMove(int _left, int _top);
		virtual void onMouseDrag(int _left, int _top);
		virtual void onMouseWheel(int _rel);
		virtual void onMouseButtonPressed(int _left, int _top, MyGUI::MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MyGUI::MouseButton _id);
		virtual void onKeyLostFocus(MyGUI::Widget* _new);
		virtual void onKeySetFocus(MyGUI::Widget* _old);

		// WebViewListener 1.0.8
		virtual void onBeginNavigation(const std::string& url, const std::wstring& frameName) { }
		virtual void onBeginLoading(const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType) { }
		virtual void onFinishLoading() { }
		virtual void onCallback(const std::string& name, const Awesomium::JSArguments& args) { }
		virtual void onReceiveTitle(const std::wstring& title, const std::wstring& frameName) { }
		virtual void onChangeTooltip(const std::wstring& tooltip) { }
#if defined(_WIN32)
		virtual void onChangeCursor(const HCURSOR& cursor);
#endif
		virtual void onChangeKeyboardFocus(bool isFocused);
		virtual void onChangeTargetURL(const std::string& url) { }

	private:
		size_t mOldCursor;
		Awesomium::WebView* mControl;
		bool mIsTransparent;
		int mOldWidth;
		int mOldHeight;
	};

} // namespace Awesomium

#endif // __AWESOMIUM_WIDGET_H__
