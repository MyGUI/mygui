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
		PointerInfo(const floatRect &_offset, const intSize & _point) : offset(_offset), point(_point) {}
		floatRect offset;
		intSize point;
	};

	typedef std::map<std::string, PointerInfo> MapPointerInfo;

} // namespace widget