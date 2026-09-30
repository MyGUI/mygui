#include "SkinTest.h"
#include "SubSkinTestSupport.h"
#include "TestRunner.h"

int main()
{
	unittest::rectangularskin::Tests tests;
	unittest::rectangularskin::addSubSkinTests(tests);
	unittest::rectangularskin::addTileRectTests(tests);
	unittest::subskin::addCommonTests<MyGUI::SubSkin>(tests);
	unittest::subskin::addCommonTests<MyGUI::MainSkin>(tests);
	unittest::subskin::addCommonTests<MyGUI::TileRect>(tests);
	return unittest::runTests(tests);
}
