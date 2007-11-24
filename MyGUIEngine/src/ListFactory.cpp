
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
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("ListAddString") = newDelegate(this, &ListFactory::ListAddString);
		}

		ListFactory::~ListFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
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