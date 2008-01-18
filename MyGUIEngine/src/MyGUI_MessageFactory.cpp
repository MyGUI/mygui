/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_MessageFactory.h"
#include "MyGUI_Message.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		MessageFactory::MessageFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			//manager.registerDelegate("Message_Pressed") = newDelegate(this, &MessageFactory::Message_Pressed);
		}

		MessageFactory::~MessageFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			//manager.unregisterDelegate("Message_Pressed");
		}

		const Ogre::String& MessageFactory::getType()
		{
			return Message::_getType();
		}

		WidgetPtr MessageFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new Message(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		/*void MessageFactory::Message_Pressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(MessagePtr, _widget);
			static_cast<MessagePtr>(_widget)->setMessagePressed(util::parseBool(_value));
		}*/

	} // namespace factory
} // namespace MyGUI
