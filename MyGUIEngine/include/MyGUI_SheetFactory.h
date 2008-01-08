/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_SHEET_FACTORY_H__
#define __MYGUI_SHEET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport SheetFactory : public WidgetFactoryInterface
		{
		public:
			SheetFactory();
			~SheetFactory();

			// ���������� ���������� �������
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_SHEET_FACTORY_H__
