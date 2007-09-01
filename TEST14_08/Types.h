#pragma once

#include <map>
#include <string>
#include <vector>
#include "TRect.h"
#include "TSize.h"
#include "TPoint.h"


namespace widget
{

	#define null 0
	#define NULL 0

	//Bit flags done easy
	#define FLAG_NONE  0
	#define FLAG(num)  (1<<(num))

	#define REGISTER_VALUE(_map,_value) _map[#_value]=_value;

} // namespace widget