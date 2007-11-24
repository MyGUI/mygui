#ifndef __EDIT_FACTORY_H__
#define __EDIT_FACTORY_H__

#include "Prerequest.h"
#include "WidgetFactoryInterface.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport EditFactory : public WidgetFactoryInterface
		{
		public:
			EditFactory::EditFactory();
			EditFactory::~EditFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name);

			// методы для парсинга
			void Edit_SetCursorPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_SetTextSelect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_SetReadOnly(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_SetPassword(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_SetMultiLine(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __EDIT_FACTORY_H__