/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"

typedef MyGUI::delegates::CDelegate1<int> HandleEvent1;
typedef MyGUI::delegates::CDelegate2<int, int> HandleEvent2;

template <typename Event1, typename Event2>
class EventPair
{
public:

	MYGUI_OBSOLETE("use signature : Event2::IDelegate * _delegate")
	void operator = (typename Event1::IDelegate * _delegate)
	{
		event1 = _delegate;
	}

	void operator = (typename Event2::IDelegate * _delegate)
	{
		event2 = _delegate;
	}

	template<typename T1, typename T2>
	void operator()(T1 _value1, T2 _value2)
	{
		event1(_value1);
		event2(_value1, _value2);
	}

private:
	Event1 event1;
	Event2 event2;
};

EventPair<HandleEvent1, HandleEvent2> Event;

void foo1(int _value1)
{
}

void foo2(int _value1, int _value2)
{
}

namespace demo
{

    void DemoKeeper::createScene()
    {
		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp", "FileSystem", "General", false, false);

		MyGUI::ResourceManager::getInstance().load("test.xml");

		//MyGUI::helper::addResourceLocation("D:/ww/WOT/Media/GUIskin/Wot", "FileSystem", "General", false, true);
		//MyGUI::helper::addResourceLocation("D:/ww/WOT/Media/images", "FileSystem", "General", false, true);
		//MyGUI::helper::addResourceLocation("D:/ww/WOT/Media/Resources", "FileSystem", "General", false, true);

		
		MyGUI::ResourceManager::getInstance().load("WOT.font");

		//MyGUI::StaticImagePtr image = MyGUI::Gui::getInstance().createWidgetReal<MyGUI::StaticImage>("StaticImage", MyGUI::FloatCoord(0, 0, 1, 1), MyGUI::Align::Default, "Main");
		//image->setImageTexture("panteonA.png");

		//Event = MyGUI::newDelegate(foo1);
		Event = MyGUI::newDelegate(foo2);
		Event(1, 2);
	}
 
    void DemoKeeper::destroyScene()
    {
    }

	 
} // namespace demo
