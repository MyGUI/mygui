/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
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
