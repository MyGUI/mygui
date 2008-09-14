/*!
	@file
	@author		Denis Koronchik
	@date		3/2008
	@module
*/

#include "MyGUI_FooBarFactory.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_FooBar.h"

namespace MyGUI
{
	namespace factory
	{

		FooBarFactory::FooBarFactory()
		{
			// register itself in manager
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);
		}

		FooBarFactory::~FooBarFactory()
		{
			// unregister itself in manager
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);
		}

		const std::string & FooBarFactory::getTypeName()
		{
			return FooBar::getClassTypeName();
		}

		WidgetPtr FooBarFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new FooBar(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

	} // namespace factory
} // namespace MyGUI
