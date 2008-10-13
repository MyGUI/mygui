
#include "StrangeButtonFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		StrangeButtonFactory::StrangeButtonFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// ������������ ��� �������
			manager.registerDelegate("StrangeButton_Pressed") = newDelegate(this, &StrangeButtonFactory::ButtonPressed);
		}

		StrangeButtonFactory::~StrangeButtonFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// ������� ��� �������
			manager.unregisterDelegate("StrangeButton_Pressed");
		}

		// ������ ��� ��������
		void StrangeButtonFactory::ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			StrangeButtonPtr widget = _widget->castType<StrangeButton>(false);
			if (widget == null) MYGUI_LOG(Error, "Property '" << _key << "' is not supported by '" << _widget->getTypeName() << "' widget");
			else widget->setButtonPressed(utility::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI