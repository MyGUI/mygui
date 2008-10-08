
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
			manager.registerDelegate("StrangeButton_Pressed") = newDelegate(this, &StrangeButtonFactory::ButtonPressed);
		}

		StrangeButtonFactory::~StrangeButtonFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("StrangeButton_Pressed");
		}

		// ���������� ���������� �������
		const std::string & StrangeButtonFactory::getTypeName()
		{
			return StrangeButton::getClassTypeName();
		}

		WidgetPtr StrangeButtonFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String& _name)
		{
			return new StrangeButton(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
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