#ifndef MYGUI_LIST_TEST_CHECKS_H_
#define MYGUI_LIST_TEST_CHECKS_H_

#include "TestSupport.h"

namespace unittest
{

	template<typename Actual, typename Expected>
	void checkEqualImpl(const Actual& _actual, const Expected& _expected, const char* _file, int _line)
	{
		if (!(_actual == _expected))
			require(false, MyGUI::utility::toString(_file, ":", _line, ": expected ", _expected, ", got ", _actual));
	}

	inline void checkDataImpl(const size_t* _actual, const size_t* _expected, const char* _file, int _line)
	{
		if ((_actual == nullptr && _expected == nullptr) ||
			(_actual != nullptr && _expected != nullptr && *_actual == *_expected))
			return;

		const auto actual = _actual == nullptr ? std::string("<null>") : MyGUI::utility::toString(*_actual);
		const auto expected = _expected == nullptr ? std::string("<null>") : MyGUI::utility::toString(*_expected);
		checkEqualImpl(actual, expected, _file, _line);
	}

}

// Descriptions document the assertion in source without being evaluated at runtime.
#define checkEqual(actual, expected, description) ::unittest::checkEqualImpl((actual), (expected), __FILE__, __LINE__)
#define checkData(actual, expected, description) ::unittest::checkDataImpl((actual), (expected), __FILE__, __LINE__)

#endif // MYGUI_LIST_TEST_CHECKS_H_
