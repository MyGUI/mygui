/*!
	@file
	@author		Albert Semenov
	@date		11/2010
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
		static const UString& getEmptyUString() { return mUString; }
		static const std::string& getEmptyString() { return mString; }
		static const IntSize& getZeroIntSize() { return mIntSize; }
		static const IntPoint& getZeroIntPoint() { return mIntPoint; }

	private:
		static UString mUString;
		static std::string mString;
		static IntSize mIntSize;
		static IntPoint mIntPoint;
	};

} // namespace MyGUI

#endif // __MYGUI_CONSTANTS_H__
