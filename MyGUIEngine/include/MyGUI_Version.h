/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_VERSION_H_
#define MYGUI_VERSION_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_StringUtility.h"

namespace MyGUI
{

	class MYGUI_EXPORT Version
	{
	public:
		Version(uint8_t _major = 0, uint8_t _minor = 0, uint16_t _patch = 0) :
			mMajor(_major),
			mMinor(_minor),
			mPatch(_patch)
		{
		}

		friend bool operator<(Version const& a, Version const& b)
		{
			return (a.mMajor < b.mMajor) ? true : (a.mMinor < b.mMinor);
		}

		friend bool operator>=(Version const& a, Version const& b)
		{
			return !(a < b);
		}

		friend bool operator>(Version const& a, Version const& b)
		{
			return (b < a);
		}

		friend bool operator<=(Version const& a, Version const& b)
		{
			return !(a > b);
		}

		friend bool operator==(Version const& a, Version const& b)
		{
			return !(a < b) && !(a > b);
		}

		friend bool operator!=(Version const& a, Version const& b)
		{
			return !(a == b);
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Version& _value)
		{
			_stream << _value.print();
			return _stream;
		}

		friend std::istream& operator>>(std::istream& _stream, Version& _value)
		{
			std::string value;
			_stream >> value;
			_value = parse(value);
			return _stream;
		}

		uint8_t getMajor() const
		{
			return mMajor;
		}

		uint8_t getMinor() const
		{
			return mMinor;
		}

		uint16_t getPatch() const
		{
			return mPatch;
		}

		std::string print() const
		{
			if (mPatch == 0)
				return utility::toString(mMajor, ".", mMinor);
			return utility::toString(mMajor, ".", mMinor, ".", mPatch);
		}

		static Version parse(std::string_view _value)
		{
			const std::vector<std::string>& vec = utility::split(_value, ".");
			if (vec.empty())
				return {};

			uint8_t major = utility::parseValue<uint8_t>(vec[0]);
			uint8_t minor = vec.size() > 1 ? utility::parseValue<uint8_t>(vec[1]) : 0;
			uint16_t patch = vec.size() > 2 ? utility::parseValue<uint16_t>(vec[2]) : 0;

			return {major, minor, patch};
		}

	private:
		uint8_t mMajor;
		uint8_t mMinor;
		uint16_t mPatch;
	};

} // namespace MyGUI

#endif // MYGUI_VERSION_H_
