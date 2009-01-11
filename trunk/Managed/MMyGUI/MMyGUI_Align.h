/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_ALIGN_H__
#define __MMYGUI_ALIGN_H__

#include <MyGUI.h>

namespace MMyGUI
{

	public value struct Align
	{

		Align( int _value ) : value( _value) { }

		static bool operator == ( Align lvalue, Align rvalue )
        {
            return ( lvalue.value == rvalue.value );
        }

        static bool operator != ( Align lvalue, Align rvalue )
        {
            return !(lvalue == rvalue);
        }

		virtual bool Equals(Align other) { return *this == other; }


		static operator MyGUI::Align& (Align& obj)
		{
			return reinterpret_cast<MyGUI::Align&>(obj);
		}

		static operator const Align& ( const MyGUI::Align& obj)
		{
			return reinterpret_cast<const Align&>(obj);
		}

		static operator const Align& ( const MyGUI::Align* pobj)	
		{
			return reinterpret_cast<const Align&>(*pobj);
		}

	private:
		int value;
	};

} // namespace MMyGUI

#endif // __MMYGUI_ALIGN_H__
