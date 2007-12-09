/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_VScrollFactory.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		VScrollFactory::VScrollFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("ScrollRange") = newDelegate(this, &VScrollFactory::ScrollRange);
			manager.registerDelegate("ScrollPosition") = newDelegate(this, &VScrollFactory::ScrollPosition);
			manager.registerDelegate("ScrollPage") = newDelegate(this, &VScrollFactory::ScrollPage);
		}

		VScrollFactory::~VScrollFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.unregisterDelegate("ScrollRange");
			manager.unregisterDelegate("ScrollPosition");
			manager.unregisterDelegate("ScrollPage");
		}

		const Ogre::String& VScrollFactory::getType()
		{
			return VScroll::getType();
		}

		WidgetPtr VScrollFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new VScroll(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void VScrollFactory::ScrollRange(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(ScrollBasePtr, _widget);
			static_cast<ScrollBasePtr>(_widget)->setScrollRange(util::parseSizeT(_value));
		}

		void VScrollFactory::ScrollPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(ScrollBasePtr, _widget);
			static_cast<ScrollBasePtr>(_widget)->setScrollPosition(util::parseSizeT(_value));
		}

		void VScrollFactory::ScrollPage(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(ScrollBasePtr, _widget);
			static_cast<ScrollBasePtr>(_widget)->setScrollPage(util::parseSizeT(_value));
		}

	} // namespace factory
} // namespace MyGUI