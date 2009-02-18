/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*//*
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
#ifndef __MYGUI_MACROS_H__
#define __MYGUI_MACROS_H__

namespace MyGUI
{

	const size_t ITEM_NONE = ((size_t)-1);
	const int DEFAULT ((int)-1);
	const float ALPHA_MAX = 1.0f;
	const float ALPHA_MIN = 0.0f;

	#define MYGUI_BIN__N(x) (x) | x>>3 | x>>6 | x>>9
	#define MYGUI_BIN__B(x) (x) & 0xf | (x)>>12 & 0xf0
	#define MYGUI_BIN8(byte) (MYGUI_BIN__B(MYGUI_BIN__N(0x##byte)))
	#define MYGUI_BIN16(byte1,byte2) ((MYGUI_BIN8(byte1)<<8)+MYGUI_BIN8(byte2))
	#define MYGUI_BIN24(byte1,byte2,byte3) ((MYGUI_BIN8(byte1)<<16)+(MYGUI_BIN8(byte2)<<8)+MYGUI_BIN8(byte3))
	#define MYGUI_BIN32(byte1,byte2,byte3,byte4) ((MYGUI_BIN8(byte1)<<24)+(MYGUI_BIN8(byte2)<<16)+(MYGUI_BIN8(byte3)<<8)+MYGUI_BIN8(byte4))

	#define MYGUI_FLAG_NONE  0
	#define MYGUI_FLAG(num)  (1<<(num))

} // namespace MyGUI


#endif // __MYGUI_MACROS_H__
