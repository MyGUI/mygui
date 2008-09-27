/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_POINTER_INFO_H__
#define __MYGUI_POINTER_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_ResourceImageSet.h"

namespace MyGUI
{

	struct PointerInfo
	{
	public:
		PointerInfo() :
			resource( 0 )
		{
		}

		PointerInfo(const FloatRect &_offset, const IntPoint & _point, const IntSize& _size, const std::string& _texture) :
			offset( _offset ),
			point( _point ),
			size( _size ),
			texture( _texture ),
			resource( 0 )
		{
		}

		PointerInfo(const IntPoint & _point, const IntSize& _size, ResourceImageSetPtr _resource) :
			point( _point ),
			size( _size ),
			resource( _resource )
		{
		}

		FloatRect offset;
		IntPoint point;
		IntSize size;
		std::string texture;

		ResourceImageSetPtr resource;
	};

	typedef std::map<std::string, PointerInfo> MapPointerInfo;

} // namespace MyGUI

#endif // __MYGUI_POINTER_INFO_H__
