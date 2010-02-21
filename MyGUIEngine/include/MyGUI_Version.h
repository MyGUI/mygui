/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_VERSION_H__
#define __MYGUI_VERSION_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_Utility.h"

namespace MyGUI
{

	class MYGUI_EXPORT Version
	{
	public:
		Version() : mMajor(0), mMinor(0), mPatch(0) { }
		Version(int _major, int _minor, int _patch) : mMajor(_major), mMinor(_minor), mPatch(_patch) { }
		Version(int _major, int _minor) : mMajor(_major), mMinor(_minor), mPatch(0) { }

		friend bool operator < (Version const& a, Version const& b) { return (a.mMajor < b.mMajor) ? true : (a.mMinor < b.mMinor); }
		friend bool operator >= (Version const& a, Version const& b) { return !(a < b); }
		friend bool operator > (Version const& a, Version const& b) { return (b < a); }
		friend bool operator <= (Version const& a, Version const& b) { return !(a > b); }

		friend bool operator == (Version const& a, Version const& b) { return !(a < b) && !(a > b); }
		friend bool operator != (Version const& a, Version const& b) { return !(a == b); }

		friend std::ostream& operator << ( std::ostream& _stream, const Version&  _value )
		{
			_stream << _value.print();
			return _stream;
		}

		friend std::istream& operator >> ( std::istream& _stream, Version&  _value )
		{
			std::string value;
			_stream >> value;
			_value = Version::parse(value);
			return _stream;
		}

		int getMajor() const { return mMajor; }
		int getMinor() const { return mMinor; }
		int getPatch() const { return mPatch; }

		std::string print() const
		{
			if (mPatch == 0) return utility::toString(mMajor, ".", mMinor);
			return utility::toString(mMajor, ".", mMinor, ".", mPatch);
		}

		static Version parse(const std::string& _value)
		{
			const std::vector<std::string>& vec = utility::split(_value, ".");
			if (vec.empty()) return Version();
			int major = utility::parseValue<int>(vec[0]);
			int minor = vec.size() > 1 ? utility::parseValue<int>(vec[1]) : 0;
			int patch = vec.size() > 2 ? utility::parseValue<int>(vec[2]) : 0;
			return Version(major, minor, patch);
		}

	private:
		unsigned mMajor : 8;
		unsigned mMinor : 8;
		unsigned mPatch : 16;
	};

} // namespace MyGUI

#endif // __MYGUI_VERSION_H__
