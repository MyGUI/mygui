#ifndef _a2586250_a98c_4b22_b1dc_62d60f6aeef9_
#define _a2586250_a98c_4b22_b1dc_62d60f6aeef9_

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

}

#endif
