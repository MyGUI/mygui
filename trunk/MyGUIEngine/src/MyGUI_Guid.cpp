/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "MyGUI_Guid.h"
#include "MyGUI_Common.h"
#if   OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include <uuid/uuid.h>
#elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <objbase.h>
#endif

namespace MyGUI
{
	const char Guid::convert_hex[64] = {0,1,2,3,4,5,6,7,8,9,64,64,64,64,64,64,64,10,11,12,13,14,15,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,10,11,12,13,14,15,64,64,64,64,64,64,64,64,64};

	Guid Guid::parse(const std::string& _value)
	{
		Guid ret;
		size_t start=0;
		// ������ �� �������� { ... }
		if (_value.size() == 38) {
			start ++;
#if MYGUI_DEBUG_MODE == 1
			if ((_value[0] != '{') || (_value[37] != '}')) {
				MYGUI_LOG(Error, "error parse guid'" << _value << "'");
				return ret;
			}
#endif
		}
		// ������ ��� ������ ...
		else if (_value.size() != 36) {
			MYGUI_LOG(Error, "error parse guid'" << _value << "'");
			return ret;
		}

#if MYGUI_DEBUG_MODE == 1
		if ((_value[start + 8] != '-') || (_value[start + 13] != '-') || (_value[start + 18] != '-') || (_value[start + 23] != '-')) {
			MYGUI_LOG(Error, "error parse guid'" << _value << "'");
			return ret;
		}
#endif

#define MYGUI_CONVERT_HEX(value) ((convert_hex[ ((value) - 48) & 0x3F]) & 0x3F)

#if MYGUI_DEBUG_MODE == 1
	#define MYGUI_CHECK_CONVERT_HEX(value) \
		{ \
			char tmp = ((value) - 48); \
			if ((tmp > 63) || (tmp < 0)) { \
				MYGUI_LOG(Error, "error parse guid'" << _value << "'"); \
				return Guid(); \
			} \
			tmp = convert_hex[tmp]; \
			if (tmp > 63) { \
				MYGUI_LOG(Error, "error parse guid'" << _value << "'"); \
				return Guid(); \
			} \
		}
#else
	#define MYGUI_CHECK_CONVERT_HEX(value)
#endif


		size_t count = 8;
		size_t pos = start;
		while (count > 0) {
			MYGUI_CHECK_CONVERT_HEX(_value[pos]);
			ret.original.data1 <<= 4;
			ret.original.data1 += MYGUI_CONVERT_HEX(_value[pos]);
			count --;
			pos ++;
		};

		count = 4;
		pos ++;
		while (count > 0) {
			MYGUI_CHECK_CONVERT_HEX(_value[pos]);
			ret.original.data2 <<= 4;
			ret.original.data2 += MYGUI_CONVERT_HEX(_value[pos]);
			count --;
			pos ++;
		};

		count = 4;
		pos ++;
		while (count > 0) {
			MYGUI_CHECK_CONVERT_HEX(_value[pos]);
			ret.original.data3 <<= 4;
			ret.original.data3 += MYGUI_CONVERT_HEX(_value[pos]);
			count --;
			pos ++;
		};

		count = 2; // ����� �� ��� ����� ��������
		pos ++;
		size_t num = 0;
		while (count > 0) {
			MYGUI_CHECK_CONVERT_HEX(_value[pos]);
			ret.original.data4[num] = MYGUI_CONVERT_HEX(_value[pos++]) << 4;
			MYGUI_CHECK_CONVERT_HEX(_value[pos]);
			ret.original.data4[num++] += MYGUI_CONVERT_HEX(_value[pos++]);
			count --;
		};

		count = 6; // ����� �� ��� ����� ��������
		pos ++;
		while (count > 0) {
			MYGUI_CHECK_CONVERT_HEX(_value[pos]);
			ret.original.data4[num] = MYGUI_CONVERT_HEX(_value[pos++]) << 4;
			MYGUI_CHECK_CONVERT_HEX(_value[pos]);
			ret.original.data4[num++] += MYGUI_CONVERT_HEX(_value[pos++]);
			count --;
		};

#undef MYGUI_CHECK_CONVERT_HEX
#undef MYGUI_CONVERT_HEX

	    return ret;
	}

	std::string Guid::print() const
	{
		const size_t SIZE = 39;
		char buff[SIZE];

		sprintf(buff, "{%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}\0", (int)(original.data1), (int)(original.data2), (int)(original.data3),
			(int)(original.data4[0]), (int)(original.data4[1]),
			(int)(original.data4[2]), (int)(original.data4[3]), (int)(original.data4[4]), (int)(original.data4[5]), (int)(original.data4[6]), (int)(original.data4[7])
			);

		return buff;
	}

	Guid Guid::generate()
	{
		Guid ret;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		HRESULT result = CoCreateGuid((GUID*)&ret.original.data1);
		MYGUI_ASSERT(S_OK == result, "Error generate GUID");
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		uuid_generate(ret.vec._data1);
#else
		//FIXME
#endif
		return ret;
	}

} // namespace MyGUI
