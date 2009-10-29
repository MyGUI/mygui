/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
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
		friend class factory::AwesomiumWidgetFactory;

		MYGUI_RTTI_CHILD_HEADER( AwesomiumWidget, Canvas );

	public:
		Awesomium::WebView* getControl() { return mControl; }

		void loadURL(const std::string& _url);

		void setTransparent(bool _value);

	protected:
		AwesomiumWidget(MyGUI::WidgetStyle _style, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const MyGUI::WidgetSkinInfoPtr _info, MyGUI::WidgetPtr _parent, MyGUI::ICroppedRectangle * _croppedParent, MyGUI::IWidgetCreator * _creator, const std::string& _name);
		virtual ~AwesomiumWidget();

		virtual void baseChangeWidgetSkin(MyGUI::WidgetSkinInfoPtr _info);

	private:
		void initialiseWidgetSkin(MyGUI::WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

		void notifyUpdateCanvas(MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _event);
		void notifyFrameStart(float _time);

		void update(bool _update = false);

		virtual void onMouseMove(int _left, int _top);
		virtual void onMouseDrag(int _left, int _top);
		virtual void onMouseWheel(int _rel);
		virtual void onMouseButtonPressed(int _left, int _top, MyGUI::MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MyGUI::MouseButton _id);
		virtual void onKeyLostFocus(MyGUI::WidgetPtr _new);
		virtual void onKeySetFocus(MyGUI::WidgetPtr _old);

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
