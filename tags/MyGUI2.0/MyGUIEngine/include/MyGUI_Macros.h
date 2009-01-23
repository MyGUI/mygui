/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_MACROS_H__
#define __MYGUI_MACROS_H__

#include <OgrePlatform.h>

namespace MyGUI
{

	#ifndef null
		#define null (0)
	#endif

	#undef ITEM_NONE
	#define ITEM_NONE ((size_t)-1)

	#define DEFAULT ((int)-1)

	#define BIN__N(x) (x) | x>>3 | x>>6 | x>>9
	#define BIN__B(x) (x) & 0xf | (x)>>12 & 0xf0
	#define BIN8(byte) (BIN__B(BIN__N(0x##byte)))
	#define BIN16(byte1,byte2) ((BIN8(byte1)<<8)+BIN8(byte2))
	#define BIN24(byte1,byte2,byte3) ((BIN8(byte1)<<16)+(BIN8(byte2)<<8)+BIN8(byte3))
	#define BIN32(byte1,byte2,byte3,byte4) ((BIN8(byte1)<<24)+(BIN8(byte2)<<16)+(BIN8(byte3)<<8)+BIN8(byte4))

	#define FLAG_NONE  0
	#define FLAG(num)  (1<<(num))

	#define ALPHA_MAX  1.0f
	#define ALPHA_MIN  0.0f

} // namespace MyGUI


#endif // __MYGUI_MACROS_H__
