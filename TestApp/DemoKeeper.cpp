/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
#include <typeinfo>

namespace formates
{
	template<typename T> inline std::string format() { return MyGUI::utility::toString("[ ", std::numeric_limits<T>::min(), " | ", std::numeric_limits<T>::max(), " ]"); }
	template<> inline std::string format<bool>() { return "[ true | false ]"; }
	template<> inline std::string format<float>() { return MyGUI::utility::toString("[ ", -std::numeric_limits<float>::max(), " | ", std::numeric_limits<float>::max(), " ]"); }
	template<> inline std::string format<double>() { return MyGUI::utility::toString("[ ", -std::numeric_limits<double>::max(), " | ", std::numeric_limits<double>::max(), " ]"); }
}

namespace demo
{

    void DemoKeeper::createScene()
    {

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
        const MyGUI::IntSize size(100, 100);
 

		base::BaseManager::getInstance().addResourceLocation("../../Media/TestApp");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper1.jpg");


		MyGUI::ResourceManager::getInstance().load("test.xml");

		MyGUI::ResourceManager::getInstance().load("ToolTip.layout");

		/*Ogre::Archive* pArch = Ogre::ArchiveManager::getSingleton().load( "../../Media", "FileSystem" );
		Ogre::StringVectorPtr vec = pArch->find("*", true, true);
		vec.setNull();
		Ogre::ArchiveManager::getSingleton().unload(pArch);*/

		int test = 0;


		MyGUI::MYGUI_OUT(formates::format<std::string>());

//		VectorString vec;
		//Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("General", "ToolTip.layout");
//		vec.reserve(pFileInfo->size());
//		for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi ) {
//			vec.push_back(fi->archive->getName() + "/" + fi->filename);
		//}
		//pFileInfo.setNull();
//		return vec;

		//MyGUI::WindowPtr window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(100, 100, 150, 260), MyGUI::Align::Default, "Overlapped");
		//window->setAutoAlpha(true);

		//Ogre::ResourceGroupManager & manager = Ogre::ResourceGroupManager::getSingleton();

		//int test = 0;
		/*MyGUI::EditPtr edit = MyGUI::Gui::getInstance().createWidget<MyGUI::Edit>("Edit", MyGUI::IntCoord(100, 100, 150, 26), MyGUI::Align::Default, "Overlapped");
		edit->setTabPrinting(true);*/


		

		/*State type;

		State type1 = 0;
		type1 = 101;
		State type2 = State::Miss;

		type1 = type2;

		type1 == type2;
		type1 == State::Miss;
		State::End == type2;
		State::End != type2;

		State test = State::Start;
		//int value = type1;

		//MyGUI::EditPtr edit = MyGUI::Gui::getInstance().createWidget<MyGUI::Edit>("Edit", MyGUI::IntCoord(100, 100, 150, 26), MyGUI::Align::Default, "Overlapped");
		//edit->eventEditTextChange = MyGUI::newDelegate(notifyEditTextChange);
		//button->setMaskPeek("mask.png");


		MyGUI::Align align;

		align == MyGUI::Align::Default;
		MyGUI::Align::Default == align;*/

		/*class A
		{
			A
		};

		std::type_info type = typeid(void);

		type = typeid(void);*/

		//MyGUI::xml::xmlDocument document;
		//document.createInfo();
		//document.createRoot("root")->addBody("&<>'\"");

		//document.open("test.xml");
		//document.save("test2.xml");

		/*MyGUI::RenderBoxPtr box = MyGUI::Gui::getInstance().createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(100, 100, 350, 350), MyGUI::Align::Default, "Overlapped");
		box->injectObject("axes.mesh");*/

		/*MyGUI::DDContainerPtr container = MyGUI::Gui::getInstance().createWidget<MyGUI::DDContainer>("Default", MyGUI::IntCoord(100, 100, 50, 50), MyGUI::Align::Default, "Overlapped");
		container->setNeedDragDrop(true);
		container->setNeedToolTip(true);
		container->eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
		container->eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
		container->eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
		container->eventDropState = MyGUI::newDelegate(this, &DemoKeeper::notifyDropState);
		container->requestDropWidgetInfo = MyGUI::newDelegate(this, &DemoKeeper::requestDropWidgetInfo);
		container->eventUpdateDropState = MyGUI::newDelegate(this, &DemoKeeper::notifyUpdateDropState);
		container->eventToolTip = MyGUI::newDelegate(this, &DemoKeeper::notifyToolTip);

		MyGUI::StaticImagePtr image = container->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, 50, 50), MyGUI::Align::Default);
		image->setNeedMouseFocus(false);
		image->setItemResource("pic_CoreMessageIcon");
		image->setItemGroup("Icons");
		image->setItemName("Error");

		mDropDimension.set(0, 0, 50, 50);
		mDropImage = MyGUI::Gui::getInstance().createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(), MyGUI::Align::Default, "DragAndDrop");
		mDropImage->hide();
		mDropImage->setItemResource("pic_CoreMessageIcon");
		mDropImage->setItemGroup("Icons");
		mDropImage->setItemName("Info");*/
 
		//MyGUI::WidgetPtr widget = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>("FAKE", MyGUI::IntCoord(100, 100, 100, 100), MyGUI::Align::Default, "Overlapped");

		/*MyGUI::FooBarPtr bar = MyGUI::Gui::getInstance().createWidget<MyGUI::FooBar>("Button", MyGUI::IntCoord(100, 100, 550, 50), MyGUI::Align::Default, "Overlapped");
		bar->setLayout(MyGUI::FooBar::FBL_SNAP_BOTTOM);
		bar->addItem("test1", "core.png", true, MyGUI::IntSize(30, 30));
		bar->addItem("test2", "core.png", true, MyGUI::IntSize(60, 30));
		bar->addItem("test3", "core.png", true, MyGUI::IntSize(90, 30));
		bar->addItem("test4", "core.png", true, MyGUI::IntSize(90, 30));
		bar->addItem("test5", "core.png", true, MyGUI::IntSize(90, 30));
		bar->addItem("test6", "core.png", true, MyGUI::IntSize(90, 30));
		bar->addItem("test7", "core.png", true, MyGUI::IntSize(90, 30));
		bar->addItem("test8", "core.png", true, MyGUI::IntSize(90, 30));*/

		//MyGUI::MessagePtr message = MyGUI::Message::createMessage("caption", "message", false, MyGUI::Message::OkCancel);
		//message->eventMessageBoxEnd = MyGUI::newDelegate(notifyMessageBoxEnd);
		//message->endMessage(MyGUI::Message::Ok);
    }
 
    void DemoKeeper::destroyScene()
    {
    }

	/*void DemoKeeper::notifyStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
	{
		_result = true;
	}

	void DemoKeeper::notifyRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
	{
		_result = _info.reseiver != null;
	}

	void DemoKeeper::notifyEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result)
	{
	}

	void DemoKeeper::notifyDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state)
	{
	}

	void DemoKeeper::notifyUpdateDropState(MyGUI::WidgetPtr _sender, MyGUI::VectorDropWidgetInfo & _items, const MyGUI::DropWidgetState & _state)
	{
		for (MyGUI::VectorDropWidgetInfo::iterator iter=_items.begin(); iter!=_items.end(); ++iter) {
			iter->item->castType<MyGUI::StaticImage>()->setItemName(_state.accept ? "Warning" : "Quest");
		}
	}

	void DemoKeeper::requestDropWidgetInfo(MyGUI::WidgetPtr _sender, MyGUI::VectorDropWidgetInfo & _items)
	{
		_items.push_back(MyGUI::DropWidgetInfo(mDropImage, mDropDimension));
	}

	void DemoKeeper::notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
	{
		MyGUI::MYGUI_OUT("ToolTip : ", (_info.type == MyGUI::ToolTipInfo::Show ? "show" : "hide"));
	}*/
 
} // namespace demo
