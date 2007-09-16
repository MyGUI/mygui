
#include "Window.h"
#include "WindowParser.h"

namespace MyGUI
{
	// создаем фабрику для этого виджета
	namespace factory { WidgetFactory<Window> WindowFactoryInstance("Window"); }
	// парсер команд
	namespace parser { WindowParser WindowParserInstance; }

	Window::Window(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		mShowSkinCaption(false),
		mShowSkinX(false),
		mShowSkinResize(false)
	{

		// парсим свойства
		const SkinParam & param = _info->getParams();
		if (!param.empty()) {
			SkinParam::const_iterator iter = param.find("SkinCaption");
			if (iter != param.end()) mSkinCaption = iter->second;
			iter = param.find("SkinX");
			if (iter != param.end()) mSkinX = iter->second;
			iter = param.find("SkinResize");
			if (iter != param.end()) mSkinResize = iter->second;
		}
	}

	void Window::showWindowCaption(bool _show)
	{
		if ( (_show == mShowSkinCaption) || (mSkinCaption.empty()) ) return;
		mShowSkinCaption = _show ;
	}

	void Window::showWindowX(bool _show)
	{
		if ( (_show == mShowSkinX) || (mSkinX.empty()) ) return;
		mShowSkinX = _show ;
	}

	void Window::showWindowResize(bool _show)
	{
		if ( (_show == mShowSkinResize) || (mSkinResize.empty()) ) return;
		mShowSkinResize = _show ;
	}

	void Window::OnMouseSetFocus(WidgetPtr _old)
	{
		Widget::OnMouseSetFocus(_old);
	}

	void Window::OnMouseLostFocus(WidgetPtr _new)
	{
		Widget::OnMouseLostFocus(_new);
	}

	void Window::OnMouseButtonPressed(bool _left)
	{
		Widget::OnMouseButtonPressed(_left);
	}

	void Window::OnMouseButtonReleased(bool _left)
	{
		Widget::OnMouseButtonReleased(_left);
	}

} // namespace MyGUI