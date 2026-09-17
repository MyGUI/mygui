#include "SkinTest.h"
#include <iostream>

int main()
{
	try
	{
		unittest::rectangularskin::runSubSkinTests();
		std::cout << "SubSkin tests passed" << std::endl;
		unittest::rectangularskin::runTileRectTests();
		std::cout << "TileRect tests passed" << std::endl;
		return 0;
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
		return 1;
	}
}
