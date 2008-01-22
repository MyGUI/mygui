/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#include "MyGUI_RenderBoxFactory.h"
#include "MyGUI_RenderBox.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		RenderBoxFactory::RenderBoxFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("RenderBox_Mesh") = newDelegate(this, &RenderBoxFactory::RenderBox_Mesh);
		}

		RenderBoxFactory::~RenderBoxFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("RenderBox_Mesh");
		}

		const Ogre::String& RenderBoxFactory::getType()
		{
			return RenderBox::_getType();
		}

		WidgetPtr RenderBoxFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new RenderBox(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void RenderBoxFactory::RenderBox_Mesh(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(RenderBoxPtr, _widget);
			//static_cast<RenderBoxPtr>(_widget)->setMesh(_value);
		}

	} // namespace factory
} // namespace MyGUI
