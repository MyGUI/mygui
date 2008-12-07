/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

#include "OgreMemoryManager.h"
#include "MyGUI_LayerKeeper.h"

namespace demo
{

	void diagnosticRenderItem(MyGUI::WidgetPtr _widget)
	{
		// это главный леер, к которому приатачена наша иерархия, если он 0 у рута,
		// ты мы висим и нас не видно
		MyGUI::LayerKeeper * layer = _widget->getLayerKeeper();

		// это наш айтем, т.е. некоя обертака, если кипер перекрывающийся, то обертка наша личная,
		// если нет, то одна обертка на всех кто в этом слое
		MyGUI::LayerItemKeeper * layer_item = _widget->getLayerItemKeeper();

		// мы рут
		if (layer) {

			if (!_widget->isRootWidget()) {
				MYGUI_EXCEPT("layer != null && !isRootWidget()");
			}

			if (!layer_item) {
				MYGUI_EXCEPT("layer != null && layer_item == null");
			}

			// проверяем соответствие кипера и его айтема
			bool exist = layer->existItem(layer_item);
			if (!exist) {
				MYGUI_EXCEPT("layer item is not exist");
			}

		}
		// мы не рут
		else {
			if (layer_item) {
				// ищем корневой леер
				MyGUI::ICroppedRectangle * parent = _widget->getCroppedParent();
				if (!parent) {
					MYGUI_EXCEPT("cropped parent == null");
				}

				while (parent->getCroppedParent()) {

					// у не рутов, не должен быть кипер
					MyGUI::LayerKeeper * layer3 = static_cast<MyGUI::WidgetPtr>(parent)->getLayerKeeper();
					if (layer3) {
						MYGUI_EXCEPT("layer != null");
					}

					parent = parent->getCroppedParent();
				};
				MyGUI::LayerKeeper * layer3 = static_cast<MyGUI::WidgetPtr>(parent)->getLayerKeeper();

				// у рута должен быть кипер
				if (!layer3) {
					MYGUI_EXCEPT("layer == null");
				}

				// проверяем соответствие кипера и его айтема
				bool exist = layer3->existItem(layer_item);
				if (!exist) {
					MYGUI_EXCEPT("layer item is not exist");
				}
				
			}
			// мы отдетачены
			else {
			}

			// проверяем все ли рендер дети отцепленны
		}
	}

	void notifyRootMouseChangeFocus(MyGUI::WidgetPtr _sender, bool _focus)
	{
		_sender->setAlpha(_focus ? 1 : 0.5);
	}

	void notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		MyGUI::WindowPtr window = *_sender->getUserData<MyGUI::WindowPtr>();
		window->destroySmooth();
	}

	void notifyRootKeyChangeFocus(MyGUI::WidgetPtr _sender, bool _focus)
	{
		_sender->setState(_focus ? "pushed" : "normal");
	}

    void DemoKeeper::createScene()
    {
		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp", "FileSystem", "General", false, false);
		MyGUI::ResourceManager::getInstance().load("test_skin.xml");

		//MyGUI::Gui::getInstance().hidePointer();
		//delete mInfo;
		//mInfo = null;//*/


		//MyGUI::WidgetPtr widget1 = mGUI->createWidget<MyGUI::Widget>("WindowCSX", MyGUI::IntCoord(200, 20, 150, 100), MyGUI::Align::Default, "Overlapped");
		//MyGUI::WidgetPtr widget2 = widget1->createWidget<MyGUI::Widget>("ButtonX", MyGUI::IntCoord(10, 10, 50, 50), MyGUI::Align::Default);

		//MyGUI::WidgetPtr widget3 = widget1->createWidgetRoot<MyGUI::Widget>("Test", MyGUI::IntCoord(50, 50, 100, 100), MyGUI::Align::Default, "Back");
		//MyGUI::WidgetPtr widget4 = widget3->createWidgetRoot<MyGUI::Widget>("ButtonV", MyGUI::IntCoord(10, 10, 50, 50), MyGUI::Align::Default, "Overlapped");

		//widget1->attachToLayer("Back");
		//widget3->detachFromWidget();

		//widget3->changeWidgetSkin("ScrollView");

		//diagnosticRenderItem(widget1);
		//diagnosticRenderItem(widget3);

		//widget1->eventRootMouseChangeFocus = MyGUI::newDelegate(notifyRootMouseChangeFocus);
		//widget3->eventRootMouseChangeFocus = MyGUI::newDelegate(notifyRootMouseChangeFocus);


		/*MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(400, 400, 200, 200), MyGUI::Align::Default, "Back");
		MyGUI::ButtonPtr button = window->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(0, 0, 20, 20), MyGUI::Align::Default);
		MyGUI::RenderBoxPtr render = window->createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(30, 30, 100, 100), MyGUI::Align::Default);
		render->setRenderTarget(this->mCamera);

		button->setUserData(window);
		button->eventMouseButtonClick = MyGUI::newDelegate(notifyMouseButtonClick);*/
		//window->destroySmooth();

		
		/*MyGUI::WidgetPtr menu = mGUI->createWidget<MyGUI::MenuCtrl>("PopupMenu", MyGUI::IntCoord(200, 20, 150, 100), MyGUI::Align::Default, "Overlapped");

		MyGUI::WidgetPtr item1 = menu->createWidget<MyGUI::MenuItem>("", MyGUI::IntCoord(), MyGUI::Align::Default);
		MyGUI::WidgetManager::getInstance().parse(item1, "Widget_Caption", "line1");
		MyGUI::WidgetManager::getInstance().parse(item1, "MenuItem_Type", "Popup");

		MyGUI::WidgetPtr item2 = menu->createWidget<MyGUI::MenuItem>("", MyGUI::IntCoord(), MyGUI::Align::Default);
		MyGUI::WidgetManager::getInstance().parse(item2, "Widget_Caption", "line2");
		MyGUI::WidgetManager::getInstance().parse(item2, "MenuItem_Type", "Normal");

		MyGUI::WidgetPtr item_child = item1->createWidget<MyGUI::MenuCtrl>("PopupMenu", MyGUI::IntCoord(), MyGUI::Align::Default);

		MyGUI::WidgetPtr subitem1 = item_child->createWidget<MyGUI::MenuItem>("", MyGUI::IntCoord(), MyGUI::Align::Default);
		MyGUI::WidgetManager::getInstance().parse(subitem1, "Widget_Caption", "subline1");

		MyGUI::WidgetPtr subitem2 = item_child->createWidget<MyGUI::MenuItem>("", MyGUI::IntCoord(), MyGUI::Align::Default);
		MyGUI::WidgetManager::getInstance().parse(subitem2, "Widget_Caption", "subline2");
		MyGUI::WidgetManager::getInstance().parse(subitem2, "MenuItem_Type", "Popup");

		MyGUI::WidgetPtr subsubmenu = subitem2->createWidget<MyGUI::MenuCtrl>("PopupMenu", MyGUI::IntCoord(), MyGUI::Align::Default);

		MyGUI::WidgetPtr subsubitem1 = subsubmenu->createWidget<MyGUI::MenuItem>("", MyGUI::IntCoord(), MyGUI::Align::Default);
		MyGUI::WidgetManager::getInstance().parse(subsubitem1, "Widget_Caption", "finish");

		menu->show();*/

		/*MyGUI::WindowPtr window_back = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(20, 20, 200, 150), MyGUI::Align::Default, "Back");
		window_back->setCaption("Back");

		MyGUI::WindowPtr window_overlapped1 = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(320, 120, 200, 150), MyGUI::Align::Default, "Overlapped");
		window_overlapped1->setCaption("Overlapped1");

		MyGUI::WindowPtr window_overlapped_child1 = window_overlapped1->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(10, 10, 100, 100), MyGUI::Align::Default);
		window_overlapped_child1->setCaption("child1");
		window_overlapped_child1->detachFromLayer();

		MyGUI::WindowPtr window_overlapped_child2 = window_overlapped1->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(20, 20, 100, 100), MyGUI::Align::Default);
		window_overlapped_child2->setCaption("child2");
		window_overlapped_child1->detachFromLayer();

		MyGUI::WindowPtr window_overlapped2 = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(460, 260, 200, 150), MyGUI::Align::Default, "Overlapped");
		window_overlapped2->setCaption("Overlapped2");

		MyGUI::WindowPtr window_main = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(620, 420, 200, 150), MyGUI::Align::Default, "Main");
		window_main->setCaption("Main");*/


		//item2->detachFromLayer();

		/*MyGUI::LayerKeeper * layer = item2->getLayerKeeper();
		MyGUI::LayerItemKeeper * layer_item = item2->getLayerItemKeeper();
		MyGUI::ICroppedRectangle * cropped = item1->getCroppedParent();*/

		MyGUI::ComboBoxPtr combo = mGUI->createWidget<MyGUI::ComboBox>("ComboBox", MyGUI::IntCoord(20, 20, 200, 26), MyGUI::Align::Default, "Main");
		combo->addItem("line1");
		combo->addItem("line2");

	}
 
    void DemoKeeper::destroyScene()
    {
    }

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		return BaseManager::keyPressed(arg);
	}
	 
} // namespace demo
