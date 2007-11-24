#ifndef __POINTER_INFO_H__
#define __POINTER_INFO_H__

#include "Prerequest.h"
#include "Types.h"

namespace MyGUI
{

	struct PointerInfo
	{
		PointerInfo()
		{
		}

		PointerInfo(const FloatRect &_offset, const IntPoint & _point) : offset(_offset), point(_point)
		{
		}

		FloatRect offset;
		IntPoint point;
	};

	typedef std::map<std::string, PointerInfo> MapPointerInfo;

} // namespace MyGUI

#endif // __POINTER_INFO_H__