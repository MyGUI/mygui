#include "SkinTest.h"
#include "TestRunner.h"

int main()
{
	unittest::customskin::Tests tests;
	unittest::customskin::addPolygonalSkinTests(tests);
	return unittest::runTests(tests);
}
