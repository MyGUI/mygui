#include "SkinTest.h"
#include <iostream>

int main(int _argc, char** _argv)
{
	if (_argc > 2)
	{
		std::cerr << "Usage: UnitTest_RotatingPolygonalSkin [test-name-substring]\n";
		return 1;
	}
	unittest::customskin::Tests tests;
	unittest::customskin::addRotatingSkinTests(tests);
	unittest::customskin::addPolygonalSkinTests(tests);
	unittest::customskin::addGeometryUtilityTests(tests);
	size_t passed = 0, failed = 0;
	for (const auto& test : tests)
	{
		if (_argc == 2 && test.name.find(_argv[1]) == std::string::npos)
			continue;
		try
		{
			test.run();
			++passed;
			std::cout << "PASS " << test.name << '\n';
		}
		catch (const std::exception& error)
		{
			++failed;
			std::cerr << "FAIL " << test.name << ": " << error.what() << '\n';
		}
	}
	std::cout << passed << " passed, " << failed << " failed\n";
	return failed == 0 && passed != 0 ? 0 : 1;
}
