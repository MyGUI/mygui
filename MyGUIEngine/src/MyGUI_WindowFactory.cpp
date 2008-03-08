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
			manager.registerDelegate("Window_MinMax") = newDelegate(this, &WindowFactory::Window_MinMax);
			manager.registerDelegate("Window_Snap") = newDelegate(this, &WindowFactory::Window_Snap);
		}

		WindowFactory::~WindowFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Window_AutoAlpha");
			manager.unregisterDelegate("Window_MinMax");
			manager.unregisterDelegate("Window_Snap");
		}

		const Ogre::String& WindowFactory::getType()
		{
			return Window::_getType();
		}

		WidgetPtr WindowFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new Window(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void WindowFactory::Window_Snap(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setSnap(utility::parseBool(_value));
		}

		void WindowFactory::Window_AutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setAutoAlpha(utility::parseBool(_value));
		}

		void WindowFactory::Window_MinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(WindowPtr, _widget, _key);
			static_cast<WindowPtr>(_widget)->setMinMax(IntRect::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI
