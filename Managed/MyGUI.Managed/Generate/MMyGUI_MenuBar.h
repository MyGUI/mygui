/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"
#include "MMyGUI_MenuCtrl.h"

namespace MMyGUI
{

	public ref class MenuBar : public MenuCtrl
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( MenuBar, MenuCtrl );

		//InsertPoint

   	public:
		Convert<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
