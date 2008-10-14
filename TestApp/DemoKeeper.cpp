/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
 
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
		//State(State const& _other) : value(_other.value) { }
		//State & operator = (State const& _other) { value = _other.value; return *this; }
		friend bool operator == (State const & a, State const & b) { return a.value == b.value; }
		friend bool operator != (State const & a, State const & b) { return a.value != b.value; }

		int value;
	};

	template <typename T>
	void checkParce(MyGUI::EditPtr _edit, size_t _count)
	{
		static const Ogre::UTFString colour = "#FF0000";
		const Ogre::UTFString & text = _edit->getOnlyText();
		size_t index = _edit->getTextCursor();

		T p;
		std::istringstream str(text);
		while (_count > 0) {
			str >> p;
			-- _count;
		}
		if (str.fail()) {
			_edit->setCaption(colour + text);
		}
		else {
			std::string tmp;
			str >> tmp;
			if (!str.fail() || tmp.find_first_not_of(" \t\r") != std::string::npos) {
				_edit->setCaption(colour + text);
			}
			else {
				_edit->setCaption(text);
			}
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
			// файл должен быть только один, если будет два, то все равно гуй его не съест
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
		checkParceFileName(edit);
		//checkParce<int>(edit, 4);
	}

    void DemoKeeper::createScene()
    {

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
        const MyGUI::IntSize size(100, 100);
 
        base::BaseManager::getInstance().addResourceLocation("../../Media/TestApp");
        base::BaseManager::getInstance().setWallpaper("wallpaper4.jpg");

		State type;

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

		MyGUI::EditPtr edit = MyGUI::Gui::getInstance().createWidget<MyGUI::Edit>("Edit", MyGUI::IntCoord(100, 100, 150, 26), MyGUI::Align::Default, "Overlapped");
		edit->eventEditTextChange = MyGUI::newDelegate(notifyEditTextChange);
		//button->setMaskPeek("mask.png");


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
 


  