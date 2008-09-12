/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __MYGUI_MENU_BAR_FACTORY_H__
#define __MYGUI_MENU_BAR_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport MenuBarFactory : public WidgetFactoryInterface
		{
		public:
			MenuBarFactory();
			~MenuBarFactory();

			// ���������� ���������� �������
			const std::string& getType();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const std::string& _name);

			// ������ ��� ��������
			void MenuBar_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_MENU_BAR_FACTORY_H__
