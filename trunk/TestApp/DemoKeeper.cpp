/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
#include <typeinfo>

 
namespace demo
{
 
	enum Type
	{
		None,
		Start,
		End,
		Miss,
		Accept,
		Refuse
	};

	struct State
	{
		enum
		{
			None,
			Start,
			End,
			Miss,
			Accept,
			Refuse
		};

		State() : value(None) { }
		State(int _value) : value(_value) { }
		friend bool operator == (State const & a, State const & b) { return a.value == b.value; }
		friend bool operator != (State const & a, State const & b) { return a.value != b.value; }

		int value;
	};

	template <typename T>
	void checkParce4(MyGUI::EditPtr _edit)
	{
		static const Ogre::UTFString colour = "#FF0000";
		const Ogre::UTFString & text = _edit->getOnlyText();
		size_t index = _edit->getTextCursor();

		T p;
		if (MyGUI::utility::parseComplex(text, p, p, p, p)) {
			_edit->setCaption(text);
		}
		else {
			_edit->setCaption(colour + text);
		}
		_edit->setTextCursor(index);
	}

	void checkParceFileName(MyGUI::EditPtr _edit)
	{
		static const Ogre::UTFString colour = "#FF0000";
		const Ogre::UTFString & text = _edit->getOnlyText();
		size_t index = _edit->getTextCursor();

		if (text.find_first_of("*?") == std::string::npos) {
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, text);
			// ôàéë äîëæåí áûòü òîëüêî îäèí, åñëè áóäåò äâà, òî âñå ðàâíî ãóé åãî íå ñúåñò
			if (pFileInfo->size() != 1) {
				_edit->setCaption(colour + text);
			}
			else {
				_edit->setCaption(text);
			}
			pFileInfo.setNull();
		}
		else {
			_edit->setCaption(colour + text);
		}
		_edit->setTextCursor(index);
	}

	void notifyEditTextChange(MyGUI::WidgetPtr _sender)
	{
		MyGUI::EditPtr edit = _sender->castType<MyGUI::Edit>();
		//checkParceFileName(edit);
		checkParce4<int>(edit);
	}

	void notifyMessageBoxEnd(MyGUI::WidgetPtr _sender, MyGUI::Message::ViewInfo _button)
	{
		MyGUI::MYGUI_OUT(_button);
	}

    void DemoKeeper::createScene()
    {

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
        const MyGUI::IntSize size(100, 100);
 
        base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper1.jpg");



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

		MyGUI::MessagePtr message = MyGUI::Message::createMessage("caption", "message", false, MyGUI::Message::OkCancel);
		message->eventMessageBoxEnd = MyGUI::newDelegate(notifyMessageBoxEnd);
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
