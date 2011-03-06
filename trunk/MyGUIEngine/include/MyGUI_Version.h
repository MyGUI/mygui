/*!
	@file
	@author		Albert Semenov
	@date		12/2008
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
#include "MyGUI_StringUtility.h"

namespace MyGUI
{

	class MYGUI_EXPORT Version
	{
	public:

		Version(unsigned int _major = 0, unsigned int _minor = 0, unsigned int _patch = 0) :
			mMajor(_major),
			mMinor(_minor),
			mPatch(_patch)
		{
		}

		friend bool operator < (Version const& a, Version const& b)
		{
			return (a.mMajor < b.mMajor) ? true : (a.mMinor < b.mMinor);
		}

		friend bool operator >= (Version const& a, Version const& b)
		{
			return !(a < b);
		}

		friend bool operator > (Version const& a, Version const& b)
		{
			return (b < a);
		}

		friend bool operator <= (Version const& a, Version const& b)
		{
			return !(a > b);
		}

		friend bool operator == (Version const& a, Version const& b)
		{
			return !(a < b) && !(a > b);
		}

		friend bool operator != (Version const& a, Version const& b)
		{
			return !(a == b);
		}

		friend std::ostream& operator << (std::ostream& _stream, const Version&  _value)
		{
			_stream << _value.print();
			return _stream;
		}

		friend std::istream& operator >> (std::istream& _stream, Version&  _value)
		{
			std::string value;
			_stream >> value;
			_value = parse(value);
			return _stream;
		}

		unsigned int getMajor() const
		{
			return mMajor;
		}

		unsigned int getMinor() const
		{
			return mMinor;
		}

		unsigned int getPatch() const
		{
			return mPatch;
		}

		std::string print() const
		{
			if (mPatch == 0)
				return utility::toString(mMajor, ".", mMinor);
			return utility::toString(mMajor, ".", mMinor, ".", mPatch);
		}

		static Version parse(const std::string& _value)
		{
			const std::vector<std::string>& vec = utility::split(_value, ".");
			if (vec.empty())
				return Version();

			unsigned int major = utility::parseValue<unsigned int>(vec[0]);
			unsigned int minor = vec.size() > 1 ? utility::parseValue<unsigned int>(vec[1]) : 0;
			unsigned int patch = vec.size() > 2 ? utility::parseValue<unsigned int>(vec[2]) : 0;

			return Version(major, minor, patch);
		}

	private:
		unsigned mMajor : 8;
		unsigned mMinor : 8;
		unsigned mPatch : 16;
	};

} // namespace MyGUI

#endif // __MYGUI_VERSION_H__
