#ifndef MYGUI_UNITTEST_TEST_RUNNER_H_
#define MYGUI_UNITTEST_TEST_RUNNER_H_

#include <exception>
#include <initializer_list>
#include <iostream>
#include <functional>
#include <string>

namespace unittest
{

	struct TestCase
	{
		std::string name;
		std::function<void()> run;
	};

	enum class FailurePolicy
	{
		Continue,
		Stop
	};

	// Report every independent scenario, even if an earlier assertion failed.
	template<typename Tests>
	int runTests(const Tests& _tests, FailurePolicy _failurePolicy = FailurePolicy::Continue)
	{
		int passed = 0;
		int failures = 0;
		for (const auto& test : _tests)
		{
			try
			{
				test.run();
				++passed;
				std::cout << "PASS: " << test.name << '\n';
			}
			catch (const std::exception& error)
			{
				++failures;
				std::cerr << "FAIL: " << test.name << ": " << error.what() << '\n';
				if (_failurePolicy == FailurePolicy::Stop)
					break;
			}
		}
		std::cout << passed << " passed, " << failures << " failed\n";
		return failures == 0 && passed != 0 ? 0 : 1;
	}

	inline int runTests(std::initializer_list<TestCase> _tests, FailurePolicy _failurePolicy = FailurePolicy::Continue)
	{
		return runTests<std::initializer_list<TestCase>>(_tests, _failurePolicy);
	}

}

#endif // MYGUI_UNITTEST_TEST_RUNNER_H_
