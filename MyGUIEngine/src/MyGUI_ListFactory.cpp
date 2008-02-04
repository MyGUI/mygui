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
			manager.registerDelegate("List_AddString") = newDelegate(this, &ListFactory::List_AddString);
		}

		ListFactory::~ListFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("List_AddString");
		}

		const Ogre::String& ListFactory::getType()
		{
			return List::_getType();
		}

		WidgetPtr ListFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new List(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void ListFactory::List_AddString(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ListPtr, _widget, _key);
			static_cast<ListPtr>(_widget)->addItem(_value);
		}

	} // namespace factory
} // namespace MyGUI
