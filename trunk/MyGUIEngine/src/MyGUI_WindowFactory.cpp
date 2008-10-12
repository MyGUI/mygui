/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_WindowFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		WindowFactory::WindowFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// ������������ ��� �������
			manager.registerDelegate("Window_AutoAlpha") = newDelegate(this, &WindowFactory::Window_AutoAlpha);
			manager.registerDelegate("Window_Snap") = newDelegate(this, &WindowFactory::Window_Snap);
			manager.registerDelegate("Window_MinMax") = newDelegate(this, &WindowFactory::Window_MinMax);
			manager.registerDelegate("Window_MinSize") = newDelegate(this, &WindowFactory::Window_MinSize);
			manager.registerDelegate("Window_MaxSize") = newDelegate(this, &WindowFactory::Window_MaxSize);
		}

		WindowFactory::~WindowFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// ������� ��� �������
			manager.unregisterDelegate("Window_AutoAlpha");
			manager.unregisterDelegate("Window_Snap");
			manager.unregisterDelegate("Window_MinMax");
			manager.unregisterDelegate("Window_MinSize");
			manager.unregisterDelegate("Window_MaxSize");
		}

		void WindowFactory::Window_AutoAlpha(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<WindowPtr>(_widget)->setAutoAlpha(utility::parseBool(_value));
		}

		void WindowFactory::Window_Snap(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<WindowPtr>(_widget)->setSnap(utility::parseBool(_value));
		}

		void WindowFactory::Window_MinMax(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<WindowPtr>(_widget)->setMinMax(IntRect::parse(_value));
		}

		void WindowFactory::Window_MinSize(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<WindowPtr>(_widget)->setMinSize(IntSize::parse(_value));
		}

		void WindowFactory::Window_MaxSize(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<WindowPtr>(_widget)->setMaxSize(IntSize::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI
