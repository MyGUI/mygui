
#include "ListFactory.h"
#include "List.h"
#include "SkinManager.h"
#include "WidgetManager.h"

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

		WidgetPtr ListFactory::createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new List(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void ListFactory::ListAddString(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(ListPtr, _widget);
			static_cast<ListPtr>(_widget)->addItemString(_value);
		}

	} // namespace factory
} // namespace MyGUI