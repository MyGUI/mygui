/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_INT_COORD_H__
#define __MMYGUI_INT_COORD_H__

#include <MyGUI.h>

namespace MMyGUI
{

	public value struct IntCoord
	{
		int left, top, width, height;

		IntCoord( int _left, int _top, int _width, int _height ) : left( _left ), top( _top ), width( _width ), height( _height ) { }

		static bool operator == ( IntCoord lvalue, IntCoord rvalue )
        {
            return ( lvalue.left == rvalue.left && lvalue.top == rvalue.top && lvalue.width == rvalue.width && lvalue.height == rvalue.height );
        }

        static bool operator != ( IntCoord lvalue, IntCoord rvalue )
        {
            return !(lvalue == rvalue);
        }

		virtual bool Equals(IntCoord other) { return *this == other; }


		static operator MyGUI::IntCoord& (IntCoord& obj)
		{
			return reinterpret_cast<MyGUI::IntCoord&>(obj);
		}

		static operator const IntCoord& ( const MyGUI::IntCoord& obj)
		{
			return reinterpret_cast<const IntCoord&>(obj);
		}

		static operator const IntCoord& ( const MyGUI::IntCoord* pobj)	
		{
			return reinterpret_cast<const IntCoord&>(*pobj);
		}

	};

} // namespace MMyGUI

#endif // __MMYGUI_INT_COORD_H__
