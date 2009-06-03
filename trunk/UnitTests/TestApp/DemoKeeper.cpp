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

		MyGUI::WindowPtr widget = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(56, 16, 300, 300), MyGUI::Align::Default, "RTT_Test");
		MyGUI::WidgetPtr widget2 = widget->createWidget<MyGUI::Widget>("EditStretch", MyGUI::IntCoord(16, 16, 164, 164), MyGUI::Align::Default, "RTT_Test");
		//widget->setAutoAlpha(true);
		//MyGUI::WidgetManager::getInstance().destroyWidget(widget);

	}

    void DemoKeeper::destroyScene()
    {
    }

	void setCornerData(MyGUI::VertexQuad& _quad, MyGUI::VertexQuad::Enum _corner, float _x, float _y, float _z, float _u, float _v, unsigned int _colour, bool _flipY)
	{
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		MyGUI::EnumeratorLayer layer = MyGUI::LayerManager::getInstance().getEnumerator();
		while(layer.next())
		{
			if (layer->getName() == "RTT_Test")
			{
				MyGUI::EnumeratorILayerNode node = layer->getEnumerator();
				while(node.next())
				{
					MyGUI::RTTLayerNode* rttnode = node->castType<MyGUI::RTTLayerNode>(false);
					if (rttnode != nullptr)
					{
						rttnode->setCacheUsing(!rttnode->getCacheUsing());

						if (rttnode->getCacheUsing())
						{
							if (rttnode->getManualVertex())
							{
								rttnode->setManualVertext(false);
							}
							else
							{
								rttnode->setManualVertext(true);
								//const MyGUI::RenderTargetIndo& info = MyGUI::RenderManager::getInstance().getInfo();

								MyGUI::VertexQuad quad = rttnode->getOriginalVertextData();
								quad.vertex[MyGUI::VertexQuad::CornerRB].colour = 0xFF333333;
								quad.vertex[MyGUI::VertexQuad::CornerRT].colour = 0xFF333333;
								quad.vertex[MyGUI::VertexQuad::CornerRT2].colour = 0xFF333333;
								rttnode->setManualVertexData(quad);
							}
						}
					}
				}
			}
		}

		return BaseManager::keyPressed( arg );
	}

} // namespace demo
