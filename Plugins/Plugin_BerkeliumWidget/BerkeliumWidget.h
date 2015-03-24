/*!
	@file
	@author		Albert Semenov
	@date		02/2010
*/
#ifndef BERKELIUM_WIDGET_H_
#define BERKELIUM_WIDGET_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Canvas.h"
#include "Buffer.h"

namespace Berkelium
{
	class Window;
}

namespace MyGUI
{

	class BerkeliumDelegate;

	class MYGUI_EXPORT_DLL BerkeliumWidget :
		public Canvas
	{
		// для вызова закрытого конструктора
		friend class BerkeliumWidgetFactory;

		MYGUI_RTTI_DERIVED( BerkeliumWidget );

	public:
		BerkeliumWidget();

		void loadURL(const std::string& _url);
		Berkelium::Window* getBerkeliumWindow();

		void setWindowDelegate(BerkeliumDelegate* _windowDelegate);

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
		void update();
		void updateSize();

	private:
		Berkelium::Window* mWindow;
		int mOldWidth;
		int mOldHeight;
		helpers::Buffer mBuffer;

		BerkeliumDelegate* mWindowDelegate;
	};

} // namespace MyGUI

#endif // BERKELIUM_WIDGET_H_
