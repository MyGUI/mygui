/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_EDIT_FACTORY_H__
#define __MYGUI_EDIT_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport EditFactory : public WidgetFactoryInterface
		{
		public:
			EditFactory();
			~EditFactory();

			// ���������� ���������� �������
			const std::string& getType();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const std::string& _name);

			// ������ ��� ��������
			void Edit_CursorPosition(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_TextSelect(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_ReadOnly(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_Password(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_MultiLine(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_PasswordChar(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_MaxTextLength(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_OverflowToTheLeft(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_Static(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_ShowVScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_ShowHScroll(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_BreakLine(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void Edit_Memo(WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_EDIT_FACTORY_H__
