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
		// это главный леер, к которому приатачена наша иерархия, если он 0,
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

    void DemoKeeper::createScene()
    {
		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp", "FileSystem", "General", false, false);
		MyGUI::ResourceManager::getInstance().load("test_skin.xml");

		//MyGUI::Gui::getInstance().hidePointer();
		//delete mInfo;
		//mInfo = null;//*/


		MyGUI::WidgetPtr widget1 = mGUI->createWidget<MyGUI::Widget>("WindowCSX", MyGUI::IntCoord(200, 20, 150, 100), MyGUI::Align::Default, "Overlapped");
		//MyGUI::WidgetPtr widget2 = widget1->createWidget<MyGUI::Widget>("ButtonX", MyGUI::IntCoord(10, 10, 50, 50), MyGUI::Align::Default);

		MyGUI::WidgetPtr widget3 = widget1->createWidgetRoot<MyGUI::Widget>("Test", MyGUI::IntCoord(50, 50, 100, 100), MyGUI::Align::Default, "Back");
		//MyGUI::WidgetPtr widget4 = widget3->createWidgetRoot<MyGUI::Widget>("ButtonV", MyGUI::IntCoord(10, 10, 50, 50), MyGUI::Align::Default, "Overlapped");

		//widget1->attachToLayer("Back");
		//widget3->detachFromWidget();

		widget3->changeWidgetSkin("ScrollView");

		diagnosticRenderItem(widget1);
		diagnosticRenderItem(widget3);

		widget1->eventRootMouseChangeFocus = MyGUI::newDelegate(notifyRootMouseChangeFocus);
		//widget3->eventRootMouseChangeFocus = MyGUI::newDelegate(notifyRootMouseChangeFocus);


		/*MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(400, 400, 200, 200), MyGUI::Align::Default, "Back");
		MyGUI::RenderBoxPtr render = window->createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(0, 0, 200, 200), MyGUI::Align::Default, "Overlapped");
		render->setRenderTarget(this->mCamera);
		window->destroySmooth();*/

		
		
		int test = 0;
	}
 
    void DemoKeeper::destroyScene()
    {
    }

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		return BaseManager::keyPressed(arg);
	}
	 
} // namespace demo
