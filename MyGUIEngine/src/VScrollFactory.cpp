
#include "VScrollFactory.h"
#include "VScroll.h"
#include "SkinManager.h"
#include "WidgetManager.h"

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
			static Ogre::String type("VScroll");
			return type;
		}

		WidgetPtr VScrollFactory::createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new VScroll(_left, _top, _width, _height, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		void VScrollFactory::ScrollRange(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(ScrollBasePtr, _widget);
			static_cast<ScrollBasePtr>(_widget)->setScrollRange(util::parseSizeT(_value));
		}

		void VScrollFactory::ScrollPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(ScrollBasePtr, _widget);
			static_cast<ScrollBasePtr>(_widget)->setScrollPosition(util::parseSizeT(_value));
		}

		void VScrollFactory::ScrollPage(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			TYPE(ScrollBasePtr, _widget);
			static_cast<ScrollBasePtr>(_widget)->setScrollPage(util::parseSizeT(_value));
		}

	} // namespace factory
} // namespace MyGUI