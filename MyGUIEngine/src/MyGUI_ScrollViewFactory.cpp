/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "MyGUI_ScrollViewFactory.h"
#include "MyGUI_ScrollView.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ScrollViewFactory::ScrollViewFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);
		}

		ScrollViewFactory::~ScrollViewFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);
		}

		const std::string& ScrollViewFactory::getType()
		{
			return ScrollView::_getType();
		}

		WidgetPtr ScrollViewFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const std::string& _name)
		{
			return new ScrollView(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

	} // namespace factory
} // namespace MyGUI
