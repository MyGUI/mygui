#include "SkinTest.h"
#include "TestRunner.h"

int main()
{
	unittest::customskin::Tests tests;
	unittest::customskin::addRotatingSkinTests(tests);
	unittest::customskin::addPolygonalSkinTests(tests);
	unittest::customskin::addGeometryUtilityTests(tests);
	return unittest::runTests(tests);
}
