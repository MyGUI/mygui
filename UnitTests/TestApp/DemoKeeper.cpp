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
		base::BaseManager()
	{
		//mPluginCfgName = "test_plugins.cfg";
		//mResourceCfgName = "test_resources.cfg";
	}

	void notifyListMouseItemActivate(MyGUI::WidgetPtr _sender, size_t _index)
	{
		_sender->castType<MyGUI::List>()->removeItemAt(_index);
	}

    void DemoKeeper::createScene()
    {
		// потемнее скин
		mGUI->load("core_theme_black_orange.xml");
		mGUI->load("core_skin.xml");

	
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

		//menu->show();*/

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
