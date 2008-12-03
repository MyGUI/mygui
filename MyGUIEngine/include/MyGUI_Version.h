/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __MYGUI_VERSION_H__
#define __MYGUI_VERSION_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_Utility.h"

namespace MyGUI
{

	struct _MyGUIExport Version
	{

		Version() : value(0) { }
		Version(uint16 _major, uint8 _minor, uint8 _patch) : value((uint32(_major) << 16) + (uint32(_minor) << 8) + uint32(_patch)) { }
		explicit Version(const std::string & _value) : value(parse(_value).value) { }

		friend bool operator == (Version const & a, Version const & b) { return a.value == b.value; }
		friend bool operator != (Version const & a, Version const & b) { return a.value != b.value; }

		friend std::ostream& operator << ( std::ostream& _stream, const Version &  _value ) {
			_stream << _value.print();
			return _stream;
		}

		friend std::istream& operator >> ( std::istream& _stream, Version &  _value ) {
			std::string value;
			_stream >> value;
			_value = Version::parse(value);
			return _stream;
		}

		uint16 getMajor() const { return uint16((value & 0xFFFF0000) >> 16); }
		uint8 getMinor() const { return uint8((value & 0x0000FF00) >> 8); }
		uint8 getPatch() const { return uint8(value & 0x000000FF); }

		std::string print() const
		{
			if (getPatch() == 0) return utility::toString(getMajor(), ".", getMinor());
			return utility::toString(getMajor(), ".", getMinor(), ".", getPatch());
		}

		static Version parse(const std::string & _value)
		{
			std::vector<std::string> & vec = utility::split(_value, ".");
			if (vec.empty()) return Version();
			uint16 major = utility::parseValue<uint16>(vec[0]);
			uint8 minor = vec.size() > 0 ? utility::parseValue<uint8>(vec[1]) : 0;
			uint8 patch = vec.size() > 1 ? utility::parseValue<uint8>(vec[2]) : 0;
			return Version(major, minor, patch);
		}

	private:
		uint32 value;
	};

} // namespace MyGUI

#endif // __MYGUI_VERSION_H__
