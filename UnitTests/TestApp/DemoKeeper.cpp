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

namespace demo
{

	DemoKeeper::DemoKeeper() :
		base::BaseManager(),
		mPopupMenu(nullptr)
	{
		//mPluginCfgName = "test_plugins.cfg";
		//mResourceCfgName = "test_resources.cfg";
	}

	void notifyListMouseItemActivate(MyGUI::WidgetPtr _sender, size_t _index)
	{
		_sender->castType<MyGUI::List>()->removeItemAt(_index);
	}

	void eventToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
	{
		MyGUI::MYGUI_OUT(_info.type == MyGUI::ToolTipInfo::Show ? "Show" : "Hide");
	}

	/*void addResource(MyGUI::xml::ElementPtr _root, const std::string& _name, const MyGUI::Guid& _id, const std::string& _texture, const std::string& _states, const std::string& _sizes)
	{
		// сначала размеры
		typedef std::vector<MyGUI::IntSize> VectorSize;
		VectorSize sizes;

		typedef std::vector<std::string> VectorString;
		VectorString vec = MyGUI::utility::split(_sizes, ",");
		if (vec.empty())
		{
			// потом взять из текстуры
			sizes.push_back(MyGUI::IntSize(100, 100));

		}
		else
		{
			for (size_t index=0; index<vec.size(); ++index)
			{
				std::string size = vec[index];
				MyGUI::utility::trim(size);
				size_t pos = size.find('x');
				if (pos == std::string::npos)
				{
					int value = MyGUI::utility::parseInt(size);
					sizes.push_back(MyGUI::IntSize(value, value));
				}
				else
				{
					int width = MyGUI::utility::parseInt(size.substr(0, pos));
					int height = MyGUI::utility::parseInt(size.substr(pos + 1));
					sizes.push_back(MyGUI::IntSize(width, height));
				}
			}
		}

		// теперь состояния
		VectorString states;
		vec = MyGUI::utility::split(_states, ",");
		if (vec.empty())
		{
			states.push_back("");
		}
		else
		{
			for (size_t index=0; index<vec.size(); ++index)
			{
				std::string state = vec[index];
				MyGUI::utility::trim(state);
				states.push_back(state);
			}
		}

		// теперь создаем ресурс
		MyGUI::xml::ElementPtr node = _root->createChild("Resource");
		node->addAttribute("type", "ResourceImageSet");
		node->addAttribute("name", _name);
		node->addAttribute("id", _id.print());

		MyGUI::IntPoint point;

		for (size_t index=0; index<sizes.size(); ++index)
		{
			MyGUI::xml::ElementPtr group = node->createChild("Group");
			group->addAttribute("name", MyGUI::utility::toString(sizes[index].width, "x", sizes[index].height));
			group->addAttribute("texture", _texture);
			group->addAttribute("size", sizes[index].print());

			for (size_t index2=0; index2<states.size(); ++index2)
			{
				MyGUI::xml::ElementPtr item = group->createChild("Index");
				std::string name = states[index2];
				size_t pos2 = name.find('=');
				if (pos2 == std::string::npos)
				{
					item->addAttribute("name", name);
					MyGUI::xml::ElementPtr frame = item->createChild("Frame");
					frame->addAttribute("point", point.print());
				}
				else
				{
					std::string name2 = name.substr(pos2 + 1);
					name = name.substr(0, pos2);
					MyGUI::IntPoint point2(0, point.top);

					// ищем индекс с именем name2
					for (size_t index3=0; index3<states.size(); ++index3)
					{
						if (states[index3] == name2)
							break;
						point2.left += sizes[index].width;
					}

					item->addAttribute("name", name);
					MyGUI::xml::ElementPtr frame = item->createChild("Frame");
					frame->addAttribute("point", point2.print());
				}

				point.left += sizes[index].width;
			}

			point.left = 0;
			point.top += sizes[index].height;


		}

	}*/

	/*size_t getImageCount(const std::string& _name)
	{
		size_t result = 0;

		char buff[MAX_PATH];
		const std::string mask = "%s%.4d.png";

		while(true)
		{
			sprintf(buff, mask.c_str(), _name.c_str(), result);
			std::ifstream stream(MyGUI::helper::getResourcePath(buff).c_str());
			if (!stream.is_open()) break;
			result ++;
		}
		return result;
	}

	bool saveAniCursor(const std::string& _name, int _rate, short _pickx, short _picky)
	{
		size_t count_frames = getImageCount(_name);

		const int size_frame = 4294;

		std::string filename = _name + ".ani";

		std::ofstream stream(filename.c_str(), std::ios_base::binary);
		if (!stream.is_open()) return false;

		struct AniHeader
		{
			char chink_RIFF[4];
			int file_size;
			char chink_ACON[4];
			char chink_anih[4];

			int width;
			int height;
			int count_frame1;
			int count_frame2;
			int data1[6];
			char chink_rate[4];
			int size_chink_rate;
		};

		AniHeader header =
		{
			{'R', 'I', 'F', 'F'},
			0,
			{'A', 'C', 'O', 'N'},
			{'a', 'n', 'i', 'h'},
			36,
			36,
			2,
			2,
			{0, 0, 0, 0, 10, 1},
			{'r', 'a', 't', 'e'},
			2
		};

		header.count_frame1 = count_frames;
		header.count_frame2 = count_frames;
		header.size_chink_rate = count_frames * 4;

		struct ListHeader
		{
			char chink_LIST[4];
			int size_chink_LIST;
			char chink_fram[4];
		};

		ListHeader list = 
		{
			{'L', 'I', 'S', 'T'},
			0,
			{'f', 'r', 'a', 'm'}
		};
		list.size_chink_LIST = size_frame * count_frames + 4;

		header.file_size = 72 + header.size_chink_rate + list.size_chink_LIST;

		stream.write((const char*)&header, sizeof(AniHeader));
		for (int index=0; index<count_frames; ++index)
		{
			stream.write((const char*)&_rate, sizeof(_rate));
		}
		stream.write((const char*)&list, sizeof(ListHeader));

		struct FrameHeader
		{
			char chink_icon[4];
			int size_chink_icon;
			int data1;
			int data2;
			short data3;
			short pickx;
			short picky;
			short data4;
			int data5[12];
		};

		FrameHeader icon =
		{
			{'i', 'c', 'o', 'n'},
			4286,
			0x00020000,
			0x20200001,
			0x0000,
			2,
			2,
			0x10A8,
			{
				0x00160000,
				0x00280000,
				0x00200000,
				0x00400000,
				0x00010000,
				0x00000020,
				0x10800000,
				0x00000000,
				0x00000000,
				0x00000000,
				0x00000000
			}
		};

		icon.pickx = _pickx;
		icon.picky = _picky;

		for (size_t index=0; index<count_frames; ++index)
		{
			char buff[MAX_PATH];
			const std::string mask = "%s%.4d.png";
			sprintf(buff, mask.c_str(), _name.c_str(), index);

			Ogre::Image image;
			image.load(buff, "General");
			image.flipAroundX();

			stream.write((const char*)&icon, sizeof(FrameHeader) - 2);

			int data_bmp[1024];
			memcpy(data_bmp, image.getData(), 1024 * 4);
			stream.write((const char*)&data_bmp, 1024 * 4);

			char tmp[128];
			memset(tmp, 0, 128);
			stream.write((const char*)&tmp, 128);
		}

		stream.close();

		return true;
	}

	bool saveResourceCursor(const std::string& _name, float _rate)
	{
		MyGUI::xml::Document doc;
		doc.createDeclaration();

		MyGUI::xml::ElementPtr root = doc.createRoot("MyGUI");
		root->addAttribute("type", "Resource");

		MyGUI::xml::ElementPtr element = root->createChild("Resource");
		element->addAttribute("type", "ResourceImageSet");
		MyGUI::Guid id = MyGUI::Guid::generate();
		element->addAttribute("name", _name);
		element->addAttribute("id", id.print());

		MyGUI::xml::ElementPtr group = element->createChild("Group");
		MyGUI::xml::ElementPtr child = group->createChild("Index");
		child->addAttribute("name", "group_0");
		child->addAttribute("rate", _rate);

		char buff[MAX_PATH];
		const std::string mask = "%s%.4d.png";

		size_t count = getImageCount(_name);

		const int width_all = 512;
		int height_all = 512;

		int cur_width = 0;
		int cur_height = 0;

		Ogre::Image image_all;
		Ogre::PixelBox box_all;

		for (size_t index=0; index<count; ++index)
		{
			sprintf(buff, mask.c_str(), _name.c_str(), index);

			Ogre::Image image;
			image.load(buff, "General");
			Ogre::PixelBox box = image.getPixelBox();

			if (image_all.getData() == 0)
			{
				image_all.load(buff, "General");

				int width = image_all.getWidth();
				int height = image_all.getHeight();

				group->addAttribute("name", MyGUI::utility::toString(width, "x", height));
				group->addAttribute("size", MyGUI::utility::toString(width, " ", height));

				int count_in_width = width_all / width;
				int count_in_height = count / count_in_width + ((count % count_in_width) != 0 ? 1 : 0);

				height_all = count_in_height * height;

				// ближайшая степени 2
				int num = 1;
				while (num >= height_all)
				{
					num <<= 1;
					height_all = num;
				}

				image_all.resize(width_all, height_all);
				box_all = image_all.getPixelBox();
			}
			else
			{
				if (image.getWidth() + cur_width >= width_all)
				{
					cur_width = 0;
					cur_height += image.getHeight();
				}
				else
				{
					cur_width += image.getWidth();
				}
			}

			MyGUI::xml::ElementPtr frame = child->createChild("Frame");
			frame->addAttribute("point", MyGUI::utility::toString(cur_width, " ", cur_height));

			Ogre::PixelBox box_dest = box_all.getSubVolume(Ogre::Box(cur_width, cur_height, image.getWidth() + cur_width, image.getHeight() + cur_height));
			image_all.scale(box, box_dest);

		}

		image_all.save(_name + ".png");

		group->addAttribute("texture", _name);

		doc.save(_name + ".xml");

		return true;
	}*/

    void DemoKeeper::createScene()
    {

		MyGUI::VScrollPtr scroll = mGUI->createWidget<MyGUI::HScroll>("HSlider", MyGUI::IntCoord(10, 10, 250, 30), MyGUI::Align::Default, "Main");
		scroll->setScrollRange(100);
		scroll->setAlpha(0.5f);
		//scroll->setBeginToClick(true);

		/*MyGUI::StaticTextPtr text = mGUI->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(10, 10, 250, 250), MyGUI::Align::Default, "Main");
		text->setTextColour(MyGUI::Colour::White);
		text->setCaption("#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'\n#FF0000unknown command : #FFFFFF\'f\'");

		MyGUI::IntRect rect = text->getSubWidgetText()->getCursorRect(307);

		text->setCaption("unknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'\nunknown command : \'f\'");

		rect = text->getSubWidgetText()->getCursorRect(307);
		int tmp = 0;*/

		/*std::string line = "#{MyGUI_MessageBox_Ok}#{MyGUI_MessageBox_Ok}";
		MyGUI::LanguageManager::getInstance().addUserTag("1", "1");
		MyGUI::LanguageManager::getInstance().addUserTag("2", "2");
		std::string result = MyGUI::LanguageManager::getInstance().replaceTags(line);

		int test = 0;*/
		//text->getCursorPosition();

		/*const std::string name = "Salvage";

		saveAniCursor(name, 12, 16, 16);
		saveResourceCursor(name, 0.1f);*/

	
		//mGUI->load("WOT.font");

		//MyGUI::EditPtr edit = mGUI->createWidget<MyGUI::Edit>("WordWrapSimple2", MyGUI::IntCoord(67, 3, 290, 32), MyGUI::Align::Default, "Main");
		//edit->setTextColour(MyGUI::Colour::White);
		//edit->setFontName("BFont");
		//edit->setCaption(L"#FF0000Собрать предмет(-ы) Часть брони корабля 1 и пообщаться c Эстела. 0/10");


		/*MyGUI::xml::Document outdoc;
		outdoc.createDeclaration();
		MyGUI::xml::ElementPtr root = outdoc.createRoot("Font");

		std::string filename = "Default.imageset";
		MyGUI::xml::Document indoc;
		indoc.open(filename);

		root->addAttribute("name", indoc.getRoot()->findAttribute("Name"));
		root->addAttribute("source", indoc.getRoot()->findAttribute("Imagefile"));
		int default_height = 0;

		MyGUI::xml::ElementEnumerator iter = indoc.getRoot()->getElementEnumerator();
		while (iter.next("Image"))
		{
			int height = MyGUI::utility::parseInt(iter->findAttribute("Height"));
			MyGUI::IntCoord coord(
				MyGUI::utility::parseInt(iter->findAttribute("XPos")) - MyGUI::utility::parseInt(iter->findAttribute("XOffset")),
				MyGUI::utility::parseInt(iter->findAttribute("YPos")),
				MyGUI::utility::parseInt(iter->findAttribute("Width")),
				height );

			if (default_height == 0 && height != 0) default_height = height;

			MyGUI::xml::ElementPtr child = root->createChild("Code");
			child->addAttribute("index", iter->findAttribute("Name"));
			child->addAttribute("coord", coord.print());
		}

		root->addAttribute("default_height", default_height);

		outdoc.save(filename + ".xml");*/

		/*MyGUI::xml::Document doc2;
		if (!doc2.open(std::string("WOT_pic_old.xml")))
		{
			return;
		}

		doc2.clear();
		doc2.createDeclaration();

		MyGUI::xml::ElementPtr root = doc2.createRoot("MyGUI");
		
		MyGUI::xml::Document doc;
		if (!doc.open(std::string("Pictures_result.xml")))
		{
			return;
		}

		MyGUI::xml::ElementEnumerator iter = doc.getRoot()->getElementEnumerator();
		while (iter.next("DataItem"))
		{
			if (iter->findAttribute("TypeID") != "4ab4b24f-3480-45a3-be03-f51411726b82") continue;
			std::string name = iter->findAttribute("Name");
			MyGUI::Guid id = MyGUI::Guid::parse(iter->findAttribute("ID"));

			std::string filename, states, sizes;
			MyGUI::xml::ElementEnumerator body = iter->getElementEnumerator();
			while(body.next())
			{
				if (body->getName() == "FileName") filename = body->getContent();
				else if (body->getName() == "States") states = body->getContent();
				else if (body->getName() == "Sizes") sizes = body->getContent();
			}

			addResource(root, name, id, filename, states, sizes);
		}
		
		doc2.save(std::string("WOT_pic_old.xml"));*/

		/*MyGUI::WidgetPtr widget = mGUI->createWidget<MyGUI::Widget>("Button", MyGUI::IntCoord(20, 20, 200, 200), MyGUI::Align::Default, "Main");
		MyGUI::WidgetPtr widget2 = widget->createWidget<MyGUI::Widget>("Button", MyGUI::IntCoord(20, 20, 100, 100), MyGUI::Align::Default, "Main");

		widget->setNeedToolTip(true);
		widget->eventToolTip = MyGUI::newDelegate(eventToolTip);

		widget2->setNeedToolTip(true);*/

		/*MyGUI::MessageBoxStyle style1 = MyGUI::MessageBoxStyle::Ok;
		MyGUI::MessageBoxStyle style2 = MyGUI::MessageBoxStyle::Cancel;

		MyGUI::MessageBoxStyle style3 = MyGUI::MessageBoxStyle::Ok | style1 | style2;

		MyGUI::MessageBoxStyle style = MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::Button4 | MyGUI::MessageBoxStyle::Button1;
		std::vector<MyGUI::MessageBoxStyle> buttons = style.getButtons();*/


		/*MyGUI::Message::createMessageBox("Message", "caption", "",
			MyGUI::MessageBoxStyle::Continue |
			MyGUI::MessageBoxStyle::Icon4
			);*/

		// потемнее скин
		/*mGUI->load("core_theme_black_orange.xml");
		mGUI->load("core_skin.xml");

		mPopupMenu = mGUI->createWidget<MyGUI::PopupMenu>("PopupMenu", MyGUI::IntCoord(0, 0, 100, 100), MyGUI::Align::Default, "Overlapped");
		mPopupMenu->addItem("line1");
		mPopupMenu->addItem("line2");*/

	
		//cli::array<MMyGUI::Widget^> childs = gcnew cli::array<MMyGUI::Widget^>();


		//MMyGUI::Widget^ widget

		//delete widget;



		/*WidgetStyle style = WidgetStyle::Popup;

		std::ostringstream os;
		os << style;

		std::istringstream is("Overlapped");
		is >> style;

		style = WidgetStyle::parse("Popup");
		std::string name = style.print();

		if (style == WidgetStyle::Child) { }
		if (WidgetStyle::Popup == style) { }

		int size = sizeof(WidgetStyle);*/

		/*MyGUI::ListPtr list = mGUI->createWidget<MyGUI::List>("List", MyGUI::IntCoord(20, 20, 200, 200), MyGUI::Align::Default, "Main");
		list->eventListMouseItemActivate = MyGUI::newDelegate(notifyListMouseItemActivate);
		list->addItem("line1");
		list->addItem("line2");
		list->addItem("line3");*/
		//MyGUI::RawRect* colour_rect = widget->getSubWidgetMain()->castType<MyGUI::RawRect>();

		//colour_rect->setRectColour(MyGUI::Colour::Green, MyGUI::Colour::Green, MyGUI::Colour::Green, MyGUI::Colour::Green);




		/*size_t start = 0, end = 0;
		MyGUI::EnumeratorLayerKeeperPtr layer = MyGUI::LayerManager::getInstance().getEnumerator();
		while (layer.next()) {
			if (layer->getName() == "Main") {
				start = layer->getItemCount();
				break;
			}
		}

		MyGUI::WidgetPtr widget = mGUI->createWidget<MyGUI::Window>("StretchRectangle", MyGUI::IntCoord(200, 200, 206, 206), MyGUI::Align::Default, "Main");
		*/
		//MyGUI::WidgetPtr widget2 = widget->createWidget<MyGUI::Widget>(MyGUI::WidgetStyle::Overlapped, "Separator1", MyGUI::IntCoord(20, 20, 26, 26), MyGUI::Align::Default);

		//MyGUI::WidgetManager::getInstance().destroyWidget(widget2);


		/*layer = MyGUI::LayerManager::getInstance().getEnumerator();
		while (layer.next()) {
			if (layer->getName() == "Main") {
				end = layer->getItemCount();
				break;
			}
		}

		mInfo->change("DIFF", MyGUI::utility::toString(end-start));*/

		/*SceneNodePtr node;

		node = mSceneMgr->getRootSceneNode()->createChildSceneNode();

		foo(node);
		foo2(node);

		node = foo(node);

		if (node) {
		}

		mSceneMgr->destroySceneNode(node->getName());*/

		//foo(node);

		//Ogre::SceneManager::get

		/*base::BaseManager::getInstance().addResourceLocation("../../Media/TestApp");
		Ogre::Root::getSingleton().loadPlugin("Plugin_ParticleFX_d");
=======
=======
		MyGUI::WidgetPtr item1 = menu->createWidget<MyGUI::MenuItem>("MenuBarButton", MyGUI::IntCoord(), MyGUI::Align::Default);
		MyGUI::WidgetManager::getInstance().parse(item1, "Widget_Caption", "line1");
		MyGUI::WidgetManager::getInstance().parse(item1, "MenuItem_Type", "Popup");

		MyGUI::WidgetPtr item2 = menu->createWidget<MyGUI::MenuItem>("MenuBarButton", MyGUI::IntCoord(), MyGUI::Align::Default);
		MyGUI::WidgetManager::getInstance().parse(item2, "Widget_Caption", "line2");
		MyGUI::WidgetManager::getInstance().parse(item2, "MenuItem_Type", "Normal");

		MyGUI::WidgetPtr item_child = item1->createWidgetT(MyGUI::WidgetStyle::Popup, "PopupMenu", "PopupMenu", MyGUI::IntCoord(), MyGUI::Align::Default, "Popup");

		MyGUI::WidgetPtr subitem1 = item_child->createWidget<MyGUI::MenuItem>("PopupMenuLine", MyGUI::IntCoord(), MyGUI::Align::Default);
		MyGUI::WidgetManager::getInstance().parse(subitem1, "Widget_Caption", "subline1");
		//MyGUI::WidgetManager::getInstance().parse(subitem1, "MenuItem_Type", "Normal");

		MyGUI::WidgetPtr subitem2 = item_child->createWidget<MyGUI::MenuItem>("PopupMenuLine", MyGUI::IntCoord(), MyGUI::Align::Default);
		MyGUI::WidgetManager::getInstance().parse(subitem2, "Widget_Caption", "subline2");
		MyGUI::WidgetManager::getInstance().parse(subitem2, "MenuItem_Type", "Popup");

		MyGUI::WidgetPtr subsubmenu = subitem2->createWidget<MyGUI::PopupMenu>(MyGUI::WidgetStyle::Popup, "PopupMenu", MyGUI::IntCoord(), MyGUI::Align::Default, "Popup");

		MyGUI::WidgetPtr subsubitem1 = subsubmenu->createWidget<MyGUI::MenuItem>("PopupMenuLine", MyGUI::IntCoord(), MyGUI::Align::Default);
		MyGUI::WidgetManager::getInstance().parse(subsubitem1, "Widget_Caption", "finish");

		//menu->setVisible(true);*/

		// создаем ботов
		/*createBot(Ogre::Vector3(-200, 0, 0));
		createBot(Ogre::Vector3(0, 0, -200));
>>>>>>> .r1507

		// одного забераем себе
		sim::IBase * object = createBot(Ogre::Vector3(0, 0, 0));
		mSimulationObjects.pop_back();

		mKinematical = object->queryType<sim::KinematicalObject>();
		mAbility = object->queryType<sim::AbilityObject>();

		// наш интерфейс
		MyGUI::ButtonPtr button1 = mGUI->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 10, 100, 26), MyGUI::Align::Default, "Main");
		button1->setCaption(L"абилко1");
		button1->eventMouseButtonClick = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonClick);
		button1->setUserString("AbilityType", "Ability1");*/

		//createBot(Ogre::Vector3(-200, 0, 0));
		//createBot(Ogre::Vector3(0, 0, -200));
	}

    void DemoKeeper::destroyScene()
    {
    }

	sim::IBase * DemoKeeper::createBot(const Ogre::Vector3& _position)
	{
		// главня частьдля удобства
		sim::IBase * object = 0;
		// создаем ентити
		sim::RenderableObject * rend = new sim::RenderableObject(mSceneMgr, "robot.mesh");
		// создаем анимационную часть
		sim::AnimatebleObject * anim = new sim::AnimatebleObject();
		// создаем кинематическую часть
		sim::KinematicalObject * kin = new sim::KinematicalObject();
		// создаем часть для эффектов
		sim::AbilityObject * ability = new sim::AbilityObject("c_Ability");

		// создаем кастомную часть объекта, в данном случае робот
		sim::RobotObject * robot = new sim::RobotObject("c_Main");

		// агрегируем части
		object = rend;
		object->addBase(anim);
		object->addBase(kin);
		object->addBase(ability);
		object->addBase(robot);

		// после агрегации инициализруем
		object->initialiseBase();

		rend->getNode()->setPosition(_position);

		mSimulationObjects.push_back(object);

		return object;
	}
 
	void DemoKeeper::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		//mAbility->notifyAbility(_sender->getUserString("AbilityType"));
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		//mPopupMenu->setPosition(100, 100);
		//mPopupMenu->setVisible(true);
		//mKinematical->keyPressed(arg.key);
		return BaseManager::keyPressed(arg);
	}

	bool DemoKeeper::keyReleased( const OIS::KeyEvent &arg )
	{
		//mKinematical->keyReleased(arg.key);
		return BaseManager::keyReleased(arg);
	}

	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{
		/*static float current_time = 0;
		current_time += evt.timeSinceLastFrame;

		if (current_time > 1) {
			current_time = 0;


			float time = context::TimeContext::getCurrentTime();
			int index = rand() % 2;
			int num = rand() % 7;
			if (num == 0) {
				sim::IKinematicalObject * kin = mSimulationObjects.at(index)->queryType<sim::IKinematicalObject>();
				kin->keyPressed(OIS::KC_W);
			}
			else if (num == 1) {
				sim::IKinematicalObject * kin = mSimulationObjects.at(index)->queryType<sim::IKinematicalObject>();
				kin->keyReleased(OIS::KC_W);
			}
			else if (num == 2) {
				sim::IKinematicalObject * kin = mSimulationObjects.at(index)->queryType<sim::IKinematicalObject>();
				kin->keyPressed(OIS::KC_SPACE);
			}

		}*/

		return BaseManager::frameStarted(evt);
	}
	 
} // namespace demo
