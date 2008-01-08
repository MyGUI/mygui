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
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("Window_AutoAlpha") = newDelegate(this, &WindowFactory::Window_AutoAlpha);
			manager.registerDelegate("Window_MinMax") = newDelegate(this, &WindowFactory::Window_MinMax);
			manager.registerDelegate("Window_ToStick") = newDelegate(this, &WindowFactory::Window_ToStick);
		}

		WindowFactory::~WindowFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("Window_AutoAlpha");
			manager.unregisterDelegate("Window_MinMax");
			manager.unregisterDelegate("Window_ToStick");
		}

		const Ogre::String& WindowFactory::getType()
		{
			return Window::_getType();
		}

		WidgetPtr WindowFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new Window(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void WindowFactory::Window_ToStick(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setIsToStick(util::parseBool(_value));
		}

		void WindowFactory::Window_AutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setAutoAlpha(util::parseBool(_value));
		}

		void WindowFactory::Window_MinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(WindowPtr, _widget);
			static_cast<WindowPtr>(_widget)->setMinMax(IntRect::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI
