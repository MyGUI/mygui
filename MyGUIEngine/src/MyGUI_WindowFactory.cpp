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
			manager.registerDelegate("WindowAutoAlpha") = newDelegate(this, &WindowFactory::WindowAutoAlpha);
			manager.registerDelegate("WindowMinMax") = newDelegate(this, &WindowFactory::WindowMinMax);
			manager.registerDelegate("WindowToStick") = newDelegate(this, &WindowFactory::WindowToStick);
		}

		WindowFactory::~WindowFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("WindowAutoAlpha");
			manager.unregisterDelegate("WindowMinMax");
			manager.unregisterDelegate("WindowToStick");
		}

		const Ogre::String& WindowFactory::getType()
		{
			return Window::getType();
		}

		WidgetPtr WindowFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new Window(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void WindowFactory::WindowToStick(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setIsToStick(util::parseBool(_value));
		}

		void WindowFactory::WindowAutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setAutoAlpha(util::parseBool(_value));
		}

		void WindowFactory::WindowMinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setMinMax(IntRect::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI