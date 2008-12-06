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
		// ��� ������� ����, � �������� ���������� ���� ��������, ���� �� 0,
		// �� �� ����� � ��� �� �����
		MyGUI::LayerKeeper * layer = _widget->getLayerKeeper();

		// ��� ��� �����, �.�. ����� ��������, ���� ����� ���������������, �� ������� ���� ������,
		// ���� ���, �� ���� ������� �� ���� ��� � ���� ����
		MyGUI::LayerItemKeeper * layer_item = _widget->getLayerItemKeeper();

		// �� ���
		if (layer) {

			if (!_widget->isRootWidget()) {
				MYGUI_EXCEPT("layer != null && !isRootWidget()");
			}

			if (!layer_item) {
				MYGUI_EXCEPT("layer != null && layer_item == null");
			}

			// ��������� ������������ ������ � ��� ������
			bool exist = layer->existItem(layer_item);
			if (!exist) {
				MYGUI_EXCEPT("layer item is not exist");
			}

		}
		// �� �� ���
		else {
			if (layer_item) {
				// ���� �������� ����
				MyGUI::ICroppedRectangle * parent = _widget->getCroppedParent();
				if (!parent) {
					MYGUI_EXCEPT("cropped parent == null");
				}

				while (parent->getCroppedParent()) {

					// � �� �����, �� ������ ���� �����
					MyGUI::LayerKeeper * layer3 = static_cast<MyGUI::WidgetPtr>(parent)->getLayerKeeper();
					if (layer3) {
						MYGUI_EXCEPT("layer != null");
					}

					parent = parent->getCroppedParent();
				};
				MyGUI::LayerKeeper * layer3 = static_cast<MyGUI::WidgetPtr>(parent)->getLayerKeeper();

				// � ���� ������ ���� �����
				if (!layer3) {
					MYGUI_EXCEPT("layer == null");
				}

				// ��������� ������������ ������ � ��� ������
				bool exist = layer3->existItem(layer_item);
				if (!exist) {
					MYGUI_EXCEPT("layer item is not exist");
				}
				
			}
			// �� ����������
			else {
			}

			// ��������� ��� �� ������ ���� ����������
		}
	}

    void DemoKeeper::createScene()
    {
		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp", "FileSystem", "General", false, false);
		MyGUI::ResourceManager::getInstance().load("test_skin.xml");

		//MyGUI::Gui::getInstance().hidePointer();
		//delete mInfo;
		//mInfo = null;//*/


		MyGUI::WidgetPtr widget1 = mGUI->createWidget<MyGUI::Widget>("Test", MyGUI::IntCoord(200, 20, 100, 100), MyGUI::Align::Default, "Overlapped");
		//MyGUI::WidgetPtr widget2 = widget1->createWidget<MyGUI::Widget>("ButtonX", MyGUI::IntCoord(10, 10, 50, 50), MyGUI::Align::Default);

		MyGUI::WidgetPtr widget3 = widget1->createWidgetRoot<MyGUI::Widget>("Test", MyGUI::IntCoord(50, 50, 100, 100), MyGUI::Align::Default, "Back");
		//MyGUI::WidgetPtr widget4 = widget3->createWidgetRoot<MyGUI::Widget>("ButtonV", MyGUI::IntCoord(10, 10, 50, 50), MyGUI::Align::Default, "Overlapped");

		//widget1->detachFromLayer();
		widget3->attachToLayer("");

		diagnosticRenderItem(widget1);
		
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
