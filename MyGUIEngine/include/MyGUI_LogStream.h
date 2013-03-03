/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_LOG_STREAM_H__
#define __MYGUI_LOG_STREAM_H__

#include "MyGUI_Prerequest.h"
#include <string.h>
#include <sstream>

namespace MyGUI
{

	class MYGUI_EXPORT LogStream
	{
	public:
		struct End { };

	public:
		std::string operator << (const End& _endl)
		{
			return mStream.str();
		}

		template <typename T>
		LogStream& operator << (T _value)
		{
			mStream << _value;
			return *this;
		}

	private:
		std::ostringstream mStream;
	};

} // namespace MyGUI

#endif // __MYGUI_LOG_STREAM_H__
