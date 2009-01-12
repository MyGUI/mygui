/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_BUTTON_H__
#define __MMYGUI_BUTTON_H__

#include <MyGUI.h>

#include "MMyGUI_Widget.h"

namespace MMyGUI
{

	public ref class Button : public Widget
	{

		//--------------------------------------------------------------------
		// ���������� ����� � �������������
		MMYGUI_DECLARE_DERIVED(Button, Widget);


		//--------------------------------------------------------------------
		// �������� ������ ����������
		MMYGUI_DECLARE_PROPERTY(ButtonPressed, bool);

	};

} // namespace MMyGUI

#endif // __MMYGUI_BUTTON_H__
