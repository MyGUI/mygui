/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_TabFactory.h"
#include "MyGUI_Tab.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		TabFactory::TabFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

		}

		TabFactory::~TabFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

		}

		const Ogre::String& TabFactory::getType()
		{
			return Tab::getType();
		}

		WidgetPtr TabFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new Tab(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}


	} // namespace factory

} // namespace MyGUI
