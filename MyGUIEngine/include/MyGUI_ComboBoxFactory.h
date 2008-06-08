/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_COMBO_BOX_FACTORY_H__
#define __MYGUI_COMBO_BOX_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport ComboBoxFactory : public WidgetFactoryInterface
		{
		public:
			ComboBoxFactory();
			~ComboBoxFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name);

			// методы для парсинга
			void ComboBox_ModeDrop(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void ComboBox_AddItem(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_COMBO_BOX_FACTORY_H__
