/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#include "MyGUI_MultiListFactory.h"
#include "MyGUI_MultiList.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		MultiListFactory::MultiListFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);
		}

		MultiListFactory::~MultiListFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);
		}

		const std::string & MultiListFactory::getTypeName()
		{
			return MultiList::getClassTypeName();
		}

		WidgetPtr MultiListFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new MultiList(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

	} // namespace factory
} // namespace MyGUI
