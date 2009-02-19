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
		MYGUIEXPORT Convert< MyGUI::StaticImage * >::Type MYGUICALL ExportButton_GetStaticImage( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::StaticImage * >::To( static_cast< MyGUI::Button * >(_native)->getStaticImage( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportButton_GetImageIndex( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Button * >(_native)->getImageIndex( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportButton_SetImageIndex( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::Button * >(_native)->setImageIndex( Convert<size_t>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportButton_GetStateCheck( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Button * >(_native)->getStateCheck( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportButton_SetStateCheck( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Button * >(_native)->setStateCheck( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportButton_GetButtonPressed( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Button * >(_native)->getButtonPressed( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportButton_SetButtonPressed( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Button * >(_native)->setButtonPressed( Convert<bool>::From( _value ) );
		}
	}
	


}
