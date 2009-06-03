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

		MyGUI::WindowPtr widget = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(56, 16, 512, 512), MyGUI::Align::Default, "RTT_Test");
		MyGUI::WidgetPtr widget2 = widget->createWidget<MyGUI::Widget>("EditStretch", MyGUI::IntCoord(116, 116, 164, 164), MyGUI::Align::Default, "RTT_Test");

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

								MyGUI::VertexQuad quad = rttnode->getOriginalVertextData();
								//quad.vertex[MyGUI::VertexQuad::CornerLT].x += 0.1;
								quad.vertex[MyGUI::VertexQuad::CornerLT].y -= 0.1;

								//quad.vertex[MyGUI::VertexQuad::CornerLB].x -= 0.1;
								quad.vertex[MyGUI::VertexQuad::CornerLB].y += 0.1;
								//quad.vertex[MyGUI::VertexQuad::CornerLB2].x -= 0.1;
								quad.vertex[MyGUI::VertexQuad::CornerLB2].y += 0.1;
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
