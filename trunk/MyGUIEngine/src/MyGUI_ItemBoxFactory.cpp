/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_ItemBoxFactory.h"
#include "MyGUI_ItemBox.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ItemBoxFactory::ItemBoxFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			//manager.registerDelegate("ItemBox_AddItem") = newDelegate(this, &ItemBoxFactory::ItemBox_AddItem);
		}

		ItemBoxFactory::~ItemBoxFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			//manager.unregisterDelegate("ItemBox_AddItem");
		}

		const Ogre::String& ItemBoxFactory::getType()
		{
			return ItemBox::_getType();
		}

		WidgetPtr ItemBoxFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new ItemBox(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		/*void ItemBoxFactory::ItemBox_AddItem(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ItemBoxPtr, _widget, _key);
			static_cast<ItemBoxPtr>(_widget)->addItemString(_value);
		}*/

	} // namespace factory
} // namespace MyGUI
