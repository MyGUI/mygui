/*!
	@file
	@author		Evmenov Georgiy
	@date		01/2008
	@module
*/
#ifndef __MYGUI_RENDER_BOX_FACTORY_H__
#define __MYGUI_RENDER_BOX_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport RenderBoxFactory : public WidgetFactoryInterface
		{
		public:
			RenderBoxFactory();
			~RenderBoxFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name);

			// методы для парсинга
			void RenderBox_Mesh(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void RenderBox_AutoRotation(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void RenderBox_AutoRotationSpeed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void RenderBox_BackgroungColour(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void RenderBox_RotationAngle(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void RenderBox_MouseRotation(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void RenderBox_Animation(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void RenderBox_ViewScale(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_RENDER_BOX_FACTORY_H__
