#include "MyGUI.h"
#include "TestSupport.h"
#include "UnitTest_List.h"
#include "UnitTest_MultiList.h"
#include "UnitTest_ListItemHeight.h"
#include "UnitTest_MultiListItemHeight.h"
#include <iostream>
#include <random>
#include "Interaction.h"

int main()
{
	unsigned int seed = 0;
	int step = -1;
	try
	{
		testMultiListInteractions();
		unittest::TestContext context;
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		unittest::testListItemHeight();
		unittest::testMultiListItemHeight();
		unittest::testListItemHeightLayout();
		for (const unsigned int currentSeed : {0u, 42u, 12345u})
		{
			seed = currentSeed;
			step = -1;
			std::mt19937 random(seed);
			unittest::UnitTest_List list(random);
			unittest::UnitTest_MultiList multiList(random);
			multiList.testSorting();
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
				list.randomStep();
				multiList.randomStep();
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
