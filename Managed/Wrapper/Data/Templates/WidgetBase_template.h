/*!
	@file
	@author		Generate utility by Albert Semenov
	@date
	@module
*/
#pragma once

#include "../MMyGUI_Core.h"

namespace MyGUI
{
	namespace Managed
	{


		public ref class #{ThisName}
		{

			//--------------------------------------------------------------------
			// объявление типов и конструкторов
			MMYGUI_DECLARE_BASE( #{ThisName}, #{ThisType} );

			#{IncludeManual}

			//InsertPoint

		};

	} // namespace Managed
} // namespace MyGUI
