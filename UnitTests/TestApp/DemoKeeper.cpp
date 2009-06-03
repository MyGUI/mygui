/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
#include "MyGUI_RTTLayerFactory.h"
#include "MyGUI_RTTLayerNode.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		base::BaseManager()
	{
	}

    void DemoKeeper::createScene()
    {

		MyGUI::LayerManager::getInstance().addLayerFactory("RTTLayer", new MyGUI::RTTLayerFactory());

		this->addResourceLocation("../../Media/UnitTests/TestApp");

		mGUI->load("test_layer.xml");

		MyGUI::WindowPtr widget = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(56, 16, 300, 300), MyGUI::Align::Default, "Test");
		MyGUI::WidgetPtr widget2 = widget->createWidget<MyGUI::Widget>("EditStretch", MyGUI::IntCoord(16, 16, 164, 164), MyGUI::Align::Default, "Test");
		//widget->setAutoAlpha(true);
		//MyGUI::WidgetManager::getInstance().destroyWidget(widget);

	}

    void DemoKeeper::destroyScene()
    {
    }

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		MyGUI::RTTLayerNode::msUseCashe = !MyGUI::RTTLayerNode::msUseCashe;
		MyGUI::RTTLayerNode::msUpdate = true;

		return BaseManager::keyPressed( arg );
	}

} // namespace demo
