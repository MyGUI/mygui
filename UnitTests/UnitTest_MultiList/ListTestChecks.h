#ifndef MYGUI_LIST_TEST_CHECKS_H_
#define MYGUI_LIST_TEST_CHECKS_H_

#include "TestSupport.h"

namespace unittest
{

	template<typename Actual, typename Expected>
	void checkEqual(const Actual& _actual, const Expected& _expected, const std::string& _where)
	{
		if (!(_actual == _expected))
			require(false, MyGUI::utility::toString(_where, ": expected ", _expected, ", got ", _actual));
	}

	inline void checkData(const size_t* _actual, const size_t* _expected, const std::string& _where)
	{
		const auto actual = _actual == nullptr ? std::string("<null>") : MyGUI::utility::toString(*_actual);
		const auto expected = _expected == nullptr ? std::string("<null>") : MyGUI::utility::toString(*_expected);
		checkEqual(actual, expected, _where);
	}

}

#endif // MYGUI_LIST_TEST_CHECKS_H_
