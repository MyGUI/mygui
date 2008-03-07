
#include "StrangeButtonFactory.h"
#include "StrangeButton.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		StrangeButtonFactory::StrangeButtonFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("StrangeButtonPressed") = newDelegate(this, &StrangeButtonFactory::ButtonPressed);
		}

		StrangeButtonFactory::~StrangeButtonFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("StrangeButtonPressed");
		}

		// ���������� ���������� �������
		const Ogre::String& StrangeButtonFactory::getType()
		{
			return StrangeButton::getType();
		}

		WidgetPtr StrangeButtonFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new StrangeButton(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		// ������ ��� ��������
		void StrangeButtonFactory::ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(StrangeButtonPtr, _widget);
			static_cast<StrangeButtonPtr>(_widget)->setButtonPressed(utility::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI