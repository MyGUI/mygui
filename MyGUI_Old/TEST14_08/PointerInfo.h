#pragma once

#include <map>
#include <string>
#include "TRect.h"
#include "TSize.h"

namespace widget
{

	struct PointerInfo
	{
		PointerInfo() {}
		PointerInfo(const floatRect &_offset, const intPoint & _point) : offset(_offset), point(_point) {}
		floatRect offset;
		intPoint point;
	};

	typedef std::map<std::string, PointerInfo> MapPointerInfo;

} // namespace widget