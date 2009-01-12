/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_LIST_H__
#define __MMYGUI_LIST_H__

#include <MyGUI.h>

#include "MMyGUI_Widget.h"

namespace MMyGUI
{

	public ref class List : public Widget
	{

		//--------------------------------------------------------------------
		// ���������� ����� � �������������
		MMYGUI_DECLARE_DERIVED(List, Widget);


		//--------------------------------------------------------------------
		// �������� ������ ����������
		//MMYGUI_DECLARE_PROPERTY(ButtonPressed, bool);


		//--------------------------------------------------------------------
		// ������ ������ ����������
		MMYGUI_DECLARE_METHOD3(void, insertItemAt, size_t, const Ogre::UTFString &, MyGUI::Any);


		//--------------------------------------------------------------------
		// �������� ������ ����������
		MMYGUI_DECLARE_DELEGATE2(ListSelectAccept, MyGUI::Widget*, size_t);
		MMYGUI_DECLARE_DELEGATE2(ListChangePosition, MyGUI::Widget*, size_t);
		MMYGUI_DECLARE_DELEGATE2(ListMouseItemActivate, MyGUI::Widget*, size_t);

	};

} // namespace MMyGUI

#endif // __MMYGUI_LIST_H__
