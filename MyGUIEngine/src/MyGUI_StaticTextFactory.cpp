/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_StaticTextFactory.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		StaticTextFactory::StaticTextFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);
		}

		StaticTextFactory::~StaticTextFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);
		}

		const Ogre::String& StaticTextFactory::getType()
		{
			static Ogre::String type("StaticText");
			return type;
		}

		WidgetPtr StaticTextFactory::createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new StaticText(_left, _top, _width, _height, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

	} // namespace factory
} // namespace MyGUI