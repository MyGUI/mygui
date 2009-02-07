/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"
#{Include}

namespace MMyGUI
{

	public ref class #{ThisName} : public #{BaseName}
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( #{ThisName}, #{ThisType}, #{BaseName} );

		#{IncludeManual}

		//InsertPoint

	};

} // namespace MMyGUI
