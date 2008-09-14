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

		const std::string & PopupMenuFactory::getTypeName()
		{
			return PopupMenu::getClassTypeName();
		}

		WidgetPtr PopupMenuFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new PopupMenu(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

	} // namespace factory
} // namespace MyGUI
