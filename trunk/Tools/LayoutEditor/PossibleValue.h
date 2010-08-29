#ifndef __POSSIBLE_VALUE_H__
#define __POSSIBLE_VALUE_H__

#include <MyGUI.h>

namespace tools
{
	struct PossibleValue;
	typedef std::vector<PossibleValue*> VectorPossibleValue;

	struct PossibleValue
	{
		std::string name;
		std::vector<std::string> values;
	};

} // namespace tools

#endif // __POSSIBLE_VALUE_H__
