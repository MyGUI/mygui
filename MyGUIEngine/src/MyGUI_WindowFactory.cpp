/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_WindowFactory.h"
#include "MyGUI_Window.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		WindowFactory::WindowFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("Window_AutoAlpha") = newDelegate(this, &WindowFactory::Window_AutoAlpha);
			manager.registerDelegate("Window_Snap") = newDelegate(this, &WindowFactory::Window_Snap);
			manager.registerDelegate("Window_MinMax") = newDelegate(this, &WindowFactory::Window_MinMax);
			manager.registerDelegate("Window_MinSize") = newDelegate(this, &WindowFactory::Window_MinSize);
			manager.registerDelegate("Window_MaxSize") = newDelegate(this, &WindowFactory::Window_MaxSize);
		}

		WindowFactory::~WindowFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Window_AutoAlpha");
			manager.unregisterDelegate("Window_Snap");
			manager.unregisterDelegate("Window_MinMax");
			manager.unregisterDelegate("Window_MinSize");
			manager.unregisterDelegate("Window_MaxSize");
		}

		const std::string& WindowFactory::getType()
		{
			return Window::_getType();
		}

		WidgetPtr WindowFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const std::string& _name)
		{
			return new Window(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void WindowFactory::Window_AutoAlpha(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setAutoAlpha(utility::parseBool(_value));
		}

		void WindowFactory::Window_Snap(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setSnap(utility::parseBool(_value));
		}

		void WindowFactory::Window_MinMax(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setMinMax(IntRect::parse(_value));
		}

		void WindowFactory::Window_MinSize(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setMinSize(IntSize::parse(_value));
		}

		void WindowFactory::Window_MaxSize(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setMaxSize(IntSize::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI
