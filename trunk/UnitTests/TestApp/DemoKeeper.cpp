/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
#include "MyGUI_TextureLayerFactory.h"
#include "MyGUI_TextureLayerNode.h"
//#include "MyGUI_TextureSubSkin.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		base::BaseManager()
	{
	}

    void DemoKeeper::createScene()
    {

		MyGUI::LayerManager::getInstance().addLayerFactory("TextureLayer", new MyGUI::TextureLayerFactory());
		//MyGUI::SubWidgetFactory<MyGUI::TextureSubSkin> mFactoryTextureSubSkin;
		//MyGUI::SubWidgetManager::getInstance().registerFactory(&mFactoryTextureSubSkin);

		this->addResourceLocation("../../Media/UnitTests/TestApp");

		mGUI->load("test_layer.xml");
		//mGUI->load("test_skin.xml");

		MyGUI::WidgetPtr widget = mGUI->createWidget<MyGUI::Widget>("EditStretch", MyGUI::IntCoord(0, 0, 300, 300), MyGUI::Align::Default, "Test");
		//MyGUI::WidgetPtr widget2 = widget->createWidget<MyGUI::Widget>("TestButton", MyGUI::IntCoord(16, 16, 64, 64), MyGUI::Align::Default, "Test");
		//MyGUI::WidgetManager::getInstance().destroyWidget(widget);

	}

    void DemoKeeper::destroyScene()
    {
    }

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		MyGUI::TextureLayerNode::msUseCashe = !MyGUI::TextureLayerNode::msUseCashe;
		MyGUI::TextureLayerNode::msUpdate = true;

		return BaseManager::keyPressed( arg );
	}

} // namespace demo
