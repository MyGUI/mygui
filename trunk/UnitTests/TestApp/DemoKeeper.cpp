/*!
	@file
	@author     Albert Semenov
	@date       08/2008
	@module
*/
#include "DemoKeeper.h"
#include "MyGUI_RTTLayer.h"
#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_LayerNodeAnimation.h"
#include "WobbleNodeAnimator.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		base::BaseManager()
	{
	}

	MyGUI::WindowPtr widget = nullptr;

	void notifyWindowButtonPressed(MyGUI::WindowPtr _sender, const std::string& _name)
	{
		if (_name == "close")
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(widget);
			widget = nullptr;
		}
		else if (_name == "check")
		{
			const MyGUI::IntCoord coord(0, 0, 1024, 768);
			const MyGUI::IntSize size(300, 300);

			if (widget->getCoord().width != coord.width)
			{
				widget->setCoord(coord);
			}
			else
			{
				widget->setCoord(coord.width / 2 - size.width / 2, coord.height / 2 - size.height / 2, size.width, size.height);
			}
		}
	}

    void DemoKeeper::createScene()
    {

		MyGUI::FactoryManager::getInstance().registryFactory<WoobleNodeAnimator>("NodeAnimator", "WobbleAnimator");
		MyGUI::FactoryManager::getInstance().registryFactory<MyGUI::RTTLayer>("Layer");

		this->addResourceLocation("../../Media/UnitTests/TestApp");
		mGUI->load("test_layer.xml");

		widget = mGUI->createWidget<MyGUI::Window>("WindowCSMX", MyGUI::IntCoord(56, 16, 300, 300), MyGUI::Align::Default, "RTT_Test");
		widget->setCaption("Vertext mode");

		//MyGUI::EditPtr text = mGUI->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(356, 316, 300, 300), MyGUI::Align::Default, "RTT_Test");
		//text->setCaption("0 1 2 3");
		//text->setFontName("ManualFont");
		//text->setTextColour(MyGUI::Colour::White);

		//MyGUI::WidgetPtr widget2 = widget->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(46, 46, 164, 164), MyGUI::Align::Default, "RTT_Test");

		widget->eventWindowButtonPressed = MyGUI::newDelegate(notifyWindowButtonPressed);

	}

    void DemoKeeper::destroyScene()
    {
    }

	void setCornerData(MyGUI::VertexQuad& _quad, MyGUI::VertexQuad::Enum _corner, float _x, float _y, float _z, float _u, float _v, unsigned int _colour, bool _flipY)
	{
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		if (widget == nullptr) return BaseManager::keyPressed( arg );

		if (arg.key == OIS::KC_1)
		{
			mCamera->setPolygonMode(Ogre::PM_SOLID);
		}
		else if (arg.key == OIS::KC_2)
		{
			mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
		}
		else if (arg.key == OIS::KC_3)
		{
			mCamera->setPolygonMode(Ogre::PM_POINTS);
		}
		else if (arg.key == OIS::KC_4)
		{
			MyGUI::EnumeratorLayer layer = MyGUI::LayerManager::getInstance().getEnumerator();
			while(layer.next())
			{
				if (layer->getName() == "RTT_Test")
				{
					//layer->castType<MyGUI::RTTLayer>()->setLayerNodeAnimation(&gCustomLayerNodeAnimation);
				}
			}
		}
		else if (arg.key == OIS::KC_SPACE)
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
								if (rttnode->getLayerNodeAnimation() != nullptr)
								{
									//rttnode->setLayerNodeAnimation(nullptr);
									widget->setCaption("RTT mode");
								}
								else
								{
									//rttnode->setLayerNodeAnimation(&gCustomLayerNodeAnimation);
									widget->setCaption("Abstract mode");
								}
							}
							else
							{
								widget->setCaption("Vertext mode");
							}

						}
					}
				}
			}
		}

		return BaseManager::keyPressed( arg );
	}

} // namespace demo
