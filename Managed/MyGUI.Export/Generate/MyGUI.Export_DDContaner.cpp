/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportDDContainer_GetNeedDragDrop( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::DDContainer * >(_native)->getNeedDragDrop( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportDDContainer_SetNeedDragDrop( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::DDContainer * >(_native)->setNeedDragDrop( Convert<bool>::From( _value ) );
		}
	}
	


}
