#include "MyGUI.h"
#include "TestSupport.h"
#include "UnitTest_List.h"
#include "UnitTest_MultiList.h"
#include <iostream>

int main()
{
	unsigned int seed = 0;
	int step = -1;
	try
	{
		unittest::TestContext context;
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		for (const unsigned int currentSeed : {0u, 42u, 12345u})
		{
			seed = currentSeed;
			step = -1;
			std::srand(seed);
			unittest::UnitTest_List list;
			unittest::UnitTest_MultiList multiList;
			// Cover empty lists, each supported operation, and clearing/reuse explicitly.
			list.checkList();
			multiList.checkMultiList();
			list.RemoveItem();
			multiList.RemoveItem();
			list.AddItem(3);
			multiList.AddItem(3);
			list.InsertItem(2);
			multiList.InsertItem(2);
			list.Begin();
			multiList.Begin();
			multiList.SwapItems();
			multiList.ChangeItems(5);
			list.RemoveItem();
			multiList.RemoveItem();
			list.RemoveAllItems();
			multiList.RemoveAllItems();
			list.AddItem();
			multiList.AddItem();
			for (step = 0; step < 2000; ++step)
			{
				list.nextFrame();
				multiList.nextFrame();
			}
		}
	}
	catch (const std::exception& error)
	{
		std::cerr << "MultiList test failed (seed " << seed << ", step " << step << "): " << error.what() << '\n';
		return 1;
	}
	return 0;
}
