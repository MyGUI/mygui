/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#include "MyGUI_DDContainerFactory.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		DDContainerFactory::DDContainerFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);
		}

		DDContainerFactory::~DDContainerFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);
		}

		const std::string & DDContainerFactory::getTypeName()
		{
			return DDContainer::getClassTypeName();
		}

		WidgetPtr DDContainerFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new DDContainer(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

	} // namespace factory
} // namespace MyGUI
