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

namespace MyGUI
{

	class MYGUI_EXPORT Guid
    {
	public:
		Guid() { fast._data1 = fast._data2 = fast._data3 = fast._data4 = 0; }
		Guid( Guid const & _value ) { *this = _value; }
		explicit Guid(const std::string& _value) { *this = parse(_value); }
		explicit Guid(unsigned char(&_id)[16]) { ::memcpy((void*)&vec._data1[0], (void*)&_id[0], 16); }

		bool operator == (Guid const & _comp) const
		{
			return _comp.fast._data1 == fast._data1
				&& _comp.fast._data2 == fast._data2
				&& _comp.fast._data3 == fast._data3
				&& _comp.fast._data4 == fast._data4;
		}

		bool operator != ( Guid const & _comp ) const
		{
			return ! (*this == _comp);
		}

		bool operator < ( Guid const & _comp ) const
		{
			if (_comp.fast._data1 < fast._data1) return true;
			else if (_comp.fast._data1 > fast._data1) return false;
			if (_comp.fast._data2 < fast._data2) return true;
			else if (_comp.fast._data2 > fast._data2) return false;
			if (_comp.fast._data3 < fast._data3) return true;
			else if (_comp.fast._data3 > fast._data3) return false;
			if (_comp.fast._data4 < fast._data4) return true;
			return false;
		}

		Guid & operator = (Guid const & _rvalue)
		{
			fast._data1 = _rvalue.fast._data1;
			fast._data2 = _rvalue.fast._data2;
			fast._data3 = _rvalue.fast._data3;
			fast._data4 = _rvalue.fast._data4;
			return *this;
		}

		bool empty() const
		{
			return fast._data1 == 0
				&& fast._data2 == 0
				&& fast._data3 == 0
				&& fast._data4 == 0;
		}

		void clear()
		{
			fast._data1 = fast._data2 = fast._data3 = fast._data4 = 0;
		}

		std::string print() const;
		static Guid parse(const std::string& _value);
		static Guid generate();

        friend std::ostream& operator << ( std::ostream& _stream, const Guid &  _value )
        {
            _stream << _value.print();
            return _stream;
        }

        friend std::istream& operator >> ( std::istream& _stream, Guid &  _value )
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

		struct _original {
			uint32 data1;
			uint16 data2, data3;
			uint8 data4[8];
		};
		struct _fast {
			uint32 _data1, _data2, _data3, _data4;
		};
		struct _vec {
			unsigned char _data1[16];
		};

		union {
			_original original;
			_fast fast;
			_vec vec;
		};

	};

} // namespace MyGUI

#endif // __MYGUI_GUID_H__
