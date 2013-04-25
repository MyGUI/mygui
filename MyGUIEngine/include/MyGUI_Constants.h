/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_CONSTANTS_H__
#define __MYGUI_CONSTANTS_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_UString.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class MYGUI_EXPORT Constants
	{
	public:
		static const UString& getEmptyUString()
		{
			return mUString;
		}
		static const std::string& getEmptyString()
		{
			return mString;
		}
		static const IntSize& getZeroIntSize()
		{
			return mIntSize;
		}
		static const IntPoint& getZeroIntPoint()
		{
			return mIntPoint;
		}

	private:
		static UString mUString;
		static std::string mString;
		static IntSize mIntSize;
		static IntPoint mIntPoint;
	};

} // namespace MyGUI

#endif // __MYGUI_CONSTANTS_H__
