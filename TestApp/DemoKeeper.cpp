/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
 
namespace demo
{
 
	void DemoKeeper::notifyStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
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
	}

    void DemoKeeper::createScene()
    {

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
        const MyGUI::IntSize size(100, 100);
 
        base::BaseManager::getInstance().addResourceLocation("../../Media/TestApp");
        base::BaseManager::getInstance().setWallpaper("wallpaper4.jpg");

		MyGUI::DDContainerPtr container = MyGUI::Gui::getInstance().createWidget<MyGUI::DDContainer>("Default", MyGUI::IntCoord(100, 100, 50, 50), MyGUI::Align::Default, "Overlapped");
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
		mDropImage->setItemName("Info");
 
    }
 
    void DemoKeeper::destroyScene()
    {
    }
 
} // namespace demo 
 


  