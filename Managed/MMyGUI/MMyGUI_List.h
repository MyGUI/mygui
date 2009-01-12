/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_LIST_H__
#define __MMYGUI_LIST_H__

#include "MMyGUI_Widget.h"

namespace MMyGUI
{

	public ref class List : public Widget
	{

		//--------------------------------------------------------------------
		// объ€вление типов и конструкторов
		MMYGUI_DECLARE_DERIVED(List, Widget);


		//--------------------------------------------------------------------
		// методы паблик интерфейса
		MMYGUI_DECLARE_METHOD_RET0(getItemCount, size_t);
		MMYGUI_DECLARE_METHOD3(insertItemAt, size_t, const Ogre::UTFString &, MyGUI::Any);
		MMYGUI_DECLARE_METHOD2(addItem, const Ogre::UTFString &, MyGUI::Any);
		MMYGUI_DECLARE_METHOD1(removeItemAt, size_t);
		MMYGUI_DECLARE_METHOD0(removeAllItems);

		MMYGUI_DECLARE_METHOD2(setItemNameAt, size_t, const Ogre::UTFString &);
		MMYGUI_DECLARE_METHOD_RET1(getItemNameAt, const Ogre::UTFString &, size_t);


		//--------------------------------------------------------------------
		// делегаты паблик интерфейса
		MMYGUI_DECLARE_DELEGATE2(ListSelectAccept, MyGUI::Widget*, size_t);
		MMYGUI_DECLARE_DELEGATE2(ListChangePosition, MyGUI::Widget*, size_t);
		MMYGUI_DECLARE_DELEGATE2(ListMouseItemActivate, MyGUI::Widget*, size_t);

	};

} // namespace MMyGUI

#endif // __MMYGUI_LIST_H__
