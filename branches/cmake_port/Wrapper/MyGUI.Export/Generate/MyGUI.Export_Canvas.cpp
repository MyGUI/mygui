/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint





   	namespace ScopeCanvasProperty_TextureManaged
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportCanvas_IsTextureManaged( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Canvas * >(_native)->isTextureManaged( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportCanvas_SetTextureManaged( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Canvas * >(_native)->setTextureManaged( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeCanvasMethod_IsTextureCreated
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportCanvas_IsTextureCreated( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Canvas * >(_native)->isTextureCreated( ) );
		}
	}



   	namespace ScopeCanvasMethod_IsTextureSrcSize
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportCanvas_IsTextureSrcSize( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Canvas * >(_native)->isTextureSrcSize( ) );
		}
	}





   


   


   


   


   	/*namespace ScopeCanvasMethod_GetTextureGroup
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportCanvas_GetTextureGroup( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Canvas * >(_native)->getTextureGroup( ) );
		}
	}*/



   	namespace ScopeCanvasMethod_GetTextureName
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportCanvas_GetTextureName( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Canvas * >(_native)->getTextureName( ) );
		}
	}



   


   	namespace ScopeCanvasMethod_GetTextureSrcSize
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize< int >>::Type MYGUICALL ExportCanvas_GetTextureSrcSize( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TSize< int >>::To( static_cast< MyGUI::Canvas * >(_native)->getTextureSrcSize( ) );
		}
	}



   	namespace ScopeCanvasMethod_GetTextureSrcHeight
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportCanvas_GetTextureSrcHeight( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Canvas * >(_native)->getTextureSrcHeight( ) );
		}
	}



   	namespace ScopeCanvasMethod_GetTextureSrcWidth
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportCanvas_GetTextureSrcWidth( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Canvas * >(_native)->getTextureSrcWidth( ) );
		}
	}



   	namespace ScopeCanvasMethod_GetTextureRealSize
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize< int >>::Type MYGUICALL ExportCanvas_GetTextureRealSize( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TSize< int >>::To( static_cast< MyGUI::Canvas * >(_native)->getTextureRealSize( ) );
		}
	}



   	namespace ScopeCanvasMethod_GetTextureRealHeight
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportCanvas_GetTextureRealHeight( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Canvas * >(_native)->getTextureRealHeight( ) );
		}
	}



   	namespace ScopeCanvasMethod_GetTextureRealWidth
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportCanvas_GetTextureRealWidth( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Canvas * >(_native)->getTextureRealWidth( ) );
		}
	}



   


   


   


   


   


   	namespace ScopeCanvasMethod_IsLocked
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportCanvas_IsLocked( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Canvas * >(_native)->isLocked( ) );
		}
	}



   	namespace ScopeCanvasMethod_Unlock
	{
		MYGUIEXPORT void MYGUICALL ExportCanvas_Unlock( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Canvas * >(_native)->unlock( );
		}
	}



   


   	namespace ScopeCanvasMethod_UpdateTexture
	{
		MYGUIEXPORT void MYGUICALL ExportCanvas_UpdateTexture( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Canvas * >(_native)->updateTexture( );
		}
	}



   


   	namespace ScopeCanvasMethod_DestroyTexture
	{
		MYGUIEXPORT void MYGUICALL ExportCanvas_DestroyTexture( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Canvas * >(_native)->destroyTexture( );
		}
	}









   


   


}
