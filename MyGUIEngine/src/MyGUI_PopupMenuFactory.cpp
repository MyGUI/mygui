/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_PopupMenuFactory.h"
#include "MyGUI_PopupMenu.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		PopupMenuFactory::PopupMenuFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);
		}

		PopupMenuFactory::~PopupMenuFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);
		}

		const Ogre::String& PopupMenuFactory::getType()
		{
			return PopupMenu::_getType();
		}

		WidgetPtr PopupMenuFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new PopupMenu(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

	} // namespace factory
} // namespace MyGUI
