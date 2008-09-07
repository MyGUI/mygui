/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_GUID_H__
#define __MYGUI_GUID_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "objbase.h"
#endif

namespace MyGUI
{

	class _MyGUIExport Guid
    {
	public:
		Guid() { fast._data1 = fast._data2 = fast._data3 = fast._data4 = 0; }
		Guid( Guid const & _value ) { *this = _value; }
		explicit Guid(const std::string& _value) { *this = parse(_value); }
		
		inline bool operator == (Guid const & _comp) const
		{
			return _comp.fast._data1 == fast._data1
				&& _comp.fast._data2 == fast._data2
				&& _comp.fast._data3 == fast._data3
				&& _comp.fast._data4 == fast._data4;
		}

		inline bool operator != ( Guid const & _comp ) const
		{
			return ! (*this == _comp);
		}

		inline bool operator < ( Guid const & _comp ) const
		{
			return _comp.fast._data1 < fast._data1
				|| _comp.fast._data2 < fast._data2
				|| _comp.fast._data3 < fast._data3
				|| _comp.fast._data4 < fast._data4;
		}

		inline Guid & operator = (Guid const & _rvalue)
		{
			fast._data1 = _rvalue.fast._data1;
			fast._data2 = _rvalue.fast._data2;
			fast._data3 = _rvalue.fast._data3;
			fast._data4 = _rvalue.fast._data4;
			return *this;
		}

		inline bool empty() const
		{
			return fast._data1 == 0
				&& fast._data2 == 0
				&& fast._data3 == 0
				&& fast._data4 == 0;
		}

		inline void clear()
		{
			fast._data1 = fast._data2 = fast._data3 = fast._data4 = 0; 
		}

		std::string print() const;
		static Guid parse(const std::string& _value);
		static Guid generate();

        inline friend std::ostream& operator << ( std::ostream& _stream, const Guid &  _value )
        {
            _stream << _value.print();
            return _stream;
        }

        inline friend std::istream& operator >> ( std::istream& _stream, Guid &  _value )
        {
			std::string value;
            _stream >> value;
			if (_stream.fail()) _value.clear();
			else _value = Guid::parse(value);
            return _stream;
        }

	private:
		// массив для быстрой конвертации
		static const char convert_hex[64];

		union {
			struct _original { 
				uint32 data1;
				uint16 data2, data3;
				uint8 data4[8];
			} original;
			struct _fast { 
				uint32 _data1, _data2, _data3, _data4;
			} fast;
		};
	};

} // namespace MyGUI

#endif // __MYGUI_GUID_H__
