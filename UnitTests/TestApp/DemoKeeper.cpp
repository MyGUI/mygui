/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"

#include "DemoKeeper.h"

#include "LoopController.h"
#include "FadeController.h"
#include "SkeletonState.h"

#include "RenderableObject.h"
#include "AnimatebleObject.h"

#include "KinematicalObject.h"
#include "RobotObject.h"

#include "MyGUI_LayerKeeper.h"

#include <io.h>

namespace demo
{

	DemoKeeper::DemoKeeper() :
		base::BaseManager()
	{
	}

	void notifyCreateWidgetItem(MyGUI::ListCtrlPtr _sender, MyGUI::WidgetPtr _item)
	{
		const MyGUI::IntSize& size = _item->getSize();

		//MyGUI::WidgetPtr widget = _item->createWidget<MyGUI::Widget>("Panel", MyGUI::IntCoord(0, 0, size.width, size.height), MyGUI::Align::Stretch);

		MyGUI::EditPtr text = _item->createWidget<MyGUI::Edit>("EditStretch2", MyGUI::IntCoord(0, 0, size.width, size.height), MyGUI::Align::Default);
		text->setTextAlign(MyGUI::Align::Center);
		//text->setVisibleHScroll(false);
		//text->setVisibleVScroll(false);
		text->setNeedMouseFocus(false);
		//text->setEditWordWrap(true);

		_item->setUserData(text);
	}

	void notifyDrawItem(MyGUI::ListCtrlPtr _sender, MyGUI::WidgetPtr _item, const MyGUI::IBDrawItemInfo & _info, MyGUI::IntCoord& _coord)
	{
		MyGUI::EditPtr text = *_item->getUserData<MyGUI::EditPtr>();

		if (_info.update)
		{
			std::string data = *_sender->getItemDataAt<std::string>(_info.index);
			text->setSize(_sender->getClientCoord().size());
			text->setCaption(data);

			const MyGUI::IntSize& size = text->getTextSize() + (text->getSize() - text->getTextRegion().size());// + MyGUI::IntSize(2, 0);
			_coord.set(0, 0, size.width, size.height);

			text->setSize(size);
		}

	}

    void DemoKeeper::createScene()
    {

		//MyGUI::EditPtr edit = mGUI->createWidget<MyGUI::Edit>("EditStretch2", MyGUI::IntCoord(10, 10, 250, 250), MyGUI::Align::Default, "Main");
		//edit->setNeedMouseFocus(false);
		//edit->setCaption("this is first item");
		//edit->setEditWordWrap(false);

		//const MyGUI::IntSize& size = edit->getTextSize() + (edit->getSize() - edit->getTextRegion().size());// + MyGUI::IntSize(2, 0);
		//edit->setSize(size.width, size.height);


		MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(10, 10, 250, 250), MyGUI::Align::Default, "Main");
		MyGUI::ListCtrlPtr list = window->createWidget<MyGUI::ListCtrl>("ListCtrl", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::Align::Stretch);
		list->requestCreateWidgetItem = MyGUI::newDelegate(notifyCreateWidgetItem);
		list->requestDrawItem = MyGUI::newDelegate(notifyDrawItem);

		std::string data = "this is first item";
		list->addItem(data);
		data = "this is second item";
		list->addItem(data);
		data = "this is 3";
		list->addItem(data);
		data = "this is 4";
		list->addItem(data);
		data = "this is 5";
		list->addItem(data);
		data = "this is 6";
		list->addItem(data);
		data = "this is 7";
		list->addItem(data);
		data = "this is 8";
		list->addItem(data);
		data = "this is 9";
		list->addItem(data);//*/

	}

    void DemoKeeper::destroyScene()
    {
    }

} // namespace demo
