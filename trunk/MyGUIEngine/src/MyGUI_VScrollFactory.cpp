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
			manager.registerDelegate("Scroll_Range") = newDelegate(this, &VScrollFactory::Scroll_Range);
			manager.registerDelegate("Scroll_Position") = newDelegate(this, &VScrollFactory::Scroll_Position);
			manager.registerDelegate("Scroll_Page") = newDelegate(this, &VScrollFactory::Scroll_Page);
			manager.registerDelegate("Scroll_ViewPage") = newDelegate(this, &VScrollFactory::Scroll_ViewPage);
		}

		VScrollFactory::~VScrollFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Scroll_Range");
			manager.unregisterDelegate("Scroll_Position");
			manager.unregisterDelegate("Scroll_Page");
		}

		const Ogre::String& VScrollFactory::getType()
		{
			return VScroll::_getType();
		}

		WidgetPtr VScrollFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new VScroll(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void VScrollFactory::Scroll_Range(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(VScrollPtr, _widget, _key);
			static_cast<VScrollPtr>(_widget)->setScrollRange(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_Position(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(VScrollPtr, _widget, _key);
			static_cast<VScrollPtr>(_widget)->setScrollPosition(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_Page(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(VScrollPtr, _widget, _key);
			static_cast<VScrollPtr>(_widget)->setScrollPage(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_ViewPage(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(VScrollPtr, _widget, _key);
			static_cast<VScrollPtr>(_widget)->setScrollViewPage(utility::parseSizeT(_value));
		}

	} // namespace factory

} // namespace MyGUI
