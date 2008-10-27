/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"

namespace demo
{

    void DemoKeeper::createScene()
    {
		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp", "FileSystem", "General", false, false);

		MyGUI::ResourceManager::getInstance().load("test.xml");

		//MyGUI::helper::addResourceLocation("D:/ww/WOT/Media/GUIskin/Wot", "FileSystem", "General", false, true);
		//MyGUI::helper::addResourceLocation("D:/ww/WOT/Media/images", "FileSystem", "General", false, true);
		//MyGUI::helper::addResourceLocation("D:/ww/WOT/Media/Resources", "FileSystem", "General", false, true);

		MyGUI::StaticImagePtr image = MyGUI::Gui::getInstance().createWidgetReal<MyGUI::StaticImage>("StaticImage", MyGUI::FloatCoord(0, 0, 1, 1), MyGUI::Align::Default, "Main");
		image->setImageTexture("panteonA.png");
	}
 
    void DemoKeeper::destroyScene()
    {
    }

	 
} // namespace demo
