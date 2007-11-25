/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_ListFactory.h"
#include "MyGUI_List.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ListFactory::ListFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("ListAddString") = newDelegate(this, &ListFactory::ListAddString);
		}

		ListFactory::~ListFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.unregisterDelegate("ListAddString");
		}

		const Ogre::String& ListFactory::getType()
		{
			static Ogre::String type("List");
			return type;
		}

		WidgetPtr ListFactory::createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new List(_left, _top, _width, _height, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void ListFactory::ListAddString(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(ListPtr, _widget);
			static_cast<ListPtr>(_widget)->addItemString(_value);
		}

	} // namespace factory
} // namespace MyGUI