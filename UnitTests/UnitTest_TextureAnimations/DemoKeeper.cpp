/*!
	@file
	@author     George Evmenov
	@date       08/2009
	@module
*/
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "MyGUI_RTTLayer.h"
#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_LayerNodeAnimation.h"
#include "WobbleNodeAnimator.h"
#include "FadeNodeAnimator.h"

namespace demo
{

	DemoKeeper::DemoKeeper()
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_TextureAnimations");
	}

    void DemoKeeper::createScene()
    {
		MyGUI::FactoryManager::getInstance().registryFactory<WobbleNodeAnimator>("NodeAnimator");
		MyGUI::FactoryManager::getInstance().registryFactory<FadeNodeAnimator>("NodeAnimator");

		MyGUI::FactoryManager::getInstance().registryFactory<MyGUI::RTTLayer>("Layer");

		getGUI()->load("test_layer.xml");

		createNewWindow();
	}

    void DemoKeeper::destroyScene()
    {
    }

	void DemoKeeper::notifyWindowButtonPressed(MyGUI::WindowPtr _sender, const std::string& _name)
	{
		if (_name == "close")
		{
			mWidgets.erase(_sender);
			MyGUI::WidgetManager::getInstance().destroyWidget(_sender);
		}
		else if (_name == "check")
		{
			const MyGUI::IntCoord coord(0, 0, 1024, 768);
			const MyGUI::IntSize size(300, 300);

			if (_sender->getCoord().width != coord.width)
			{
				_sender->setCoord(coord);
			}
			else
			{
				_sender->setCoord(coord.width / 2 - size.width / 2, coord.height / 2 - size.height / 2, size.width, size.height);
			}
		}
	}

	void DemoKeeper::createNewWindow()
	{
		MyGUI::Window* widget = getGUI()->createWidget<MyGUI::Window>("WindowCSMX", MyGUI::IntCoord(56, 16, 300, 300), MyGUI::Align::Default, "RTT_Test");
		widget->setCaption("Window"/*"RTT mode"*/);
		widget->eventWindowButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::notifyWindowButtonPressed);

		mWidgets.insert(widget);
	}

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (_key == MyGUI::KeyCode::H)
		{
			createNewWindow();
		}

#ifdef MYGUI_OGRE_PLATFORM
		if (_key == MyGUI::KeyCode::One)
		{
			getCamera()->setPolygonMode(Ogre::PM_SOLID);
		}
		else if (_key == MyGUI::KeyCode::Two)
		{
			getCamera()->setPolygonMode(Ogre::PM_WIREFRAME);
		}
		else if (_key == MyGUI::KeyCode::Three)
		{
			getCamera()->setPolygonMode(Ogre::PM_POINTS);
		}
		else
#endif
			if (_key == MyGUI::KeyCode::Four)
		{
			MyGUI::LayerManager::EnumeratorLayer layer = MyGUI::LayerManager::getInstance().getEnumerator();
			while(layer.next())
			{
				if (layer->getName() == "RTT_Test")
				{
					//layer->castType<MyGUI::RTTLayer>()->setLayerNodeAnimation(&gCustomLayerNodeAnimation);
				}
			}
		}
		else if (_key == MyGUI::KeyCode::Space)
		{
			MyGUI::LayerManager::EnumeratorLayer layer = MyGUI::LayerManager::getInstance().getEnumerator();
			while(layer.next())
			{
				if (layer->getName() == "RTT_Test")
				{
					MyGUI::EnumeratorILayerNode node = layer->getEnumerator();
					while (node.next())
					{
						MyGUI::RTTLayerNode* rttnode = node->castType<MyGUI::RTTLayerNode>(false);
						if (rttnode != nullptr)
						{
							bool rtt = !rttnode->getCacheUsing();
							rttnode->setCacheUsing(rtt);

							//MyGUI::Widget* widget = rttnode->castType<MyGUI::Widget>(false);
							//if (widget != nullptr)
								//widget->setCaption(rtt ? "RTT mode" : "Vertext mode");


							/*for (SetWidget::iterator item=mWidgets.begin(); item!=mWidgets.end(); ++item)
							{
								(*item)->setCaption(mIsRTT ? "RTT mode" : "Vertext mode");
							}*/
						}
					}
				}
			}
		}

		return BaseManager::injectKeyPress( _key, _text );
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
