#include "SkinTest.h"
#include "TestRunner.h"

int main()
{
	unittest::rectangularskin::Tests tests;
	unittest::rectangularskin::addSubSkinTests(tests);
	unittest::rectangularskin::addTileRectTests(tests);
	return unittest::runTests(tests);
}
