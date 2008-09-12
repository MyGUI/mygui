/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_ButtonFactory.h"
#include "MyGUI_Button.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ButtonFactory::ButtonFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("Button_Pressed") = newDelegate(this, &ButtonFactory::Button_Pressed);
		}

		ButtonFactory::~ButtonFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("Button_Pressed");
		}

		const std::string& ButtonFactory::getType()
		{
			return Button::_getType();
		}

		WidgetPtr ButtonFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const std::string& _name)
		{
			return new Button(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void ButtonFactory::Button_Pressed(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ButtonPtr, _widget, _key);
			static_cast<ButtonPtr>(_widget)->setButtonPressed(utility::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI
