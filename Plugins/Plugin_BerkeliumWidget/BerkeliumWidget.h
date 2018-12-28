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

		MYGUI_RTTI_DERIVED( BerkeliumWidget )

	public:
		BerkeliumWidget();

		void loadURL(const std::string& _url);
		Berkelium::Window* getBerkeliumWindow();

		void setWindowDelegate(BerkeliumDelegate* _windowDelegate);

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void setPropertyOverride(const std::string& _key, const std::string& _value) override;

		void onMouseDrag(int _left, int _top, MouseButton _id) override;
		void onMouseMove(int _left, int _top) override;
		void onMouseWheel(int _rel) override;
		void onMouseButtonPressed(int _left, int _top, MouseButton _id) override;
		void onMouseButtonReleased(int _left, int _top, MouseButton _id) override;
		void onKeyLostFocus(Widget* _new) override;
		void onKeySetFocus(Widget* _old) override;
		void onKeyButtonPressed(KeyCode _key, Char _char) override;
		void onKeyButtonReleased(KeyCode _key) override;

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
