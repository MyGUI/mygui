/*!
	@file
	@author     George Evmenov
	@date       08/2009
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
		base::BaseDemoManager::createScene();
		//MyGUI::ResourceManager::getInstance().load("WindowSkin.xml");

		MyGUI::FactoryManager::getInstance().registerFactory<WobbleNodeAnimator>("NodeAnimator");
		MyGUI::FactoryManager::getInstance().registerFactory<FadeNodeAnimator>("NodeAnimator");

		std::string layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>(layerCategory);

		MyGUI::ResourceManager::getInstance().load("Layers.xml");

		createNewWindow();
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::FactoryManager::getInstance().unregisterFactory<WobbleNodeAnimator>("NodeAnimator");
		MyGUI::FactoryManager::getInstance().unregisterFactory<FadeNodeAnimator>("NodeAnimator");

		std::string layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>(layerCategory);
	}

	void DemoKeeper::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		if (_name == "close")
		{
			mWidgets.erase(_sender);
			MyGUI::WidgetManager::getInstance().destroyWidget(_sender);
		}
		/*else if (_name == "check")
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
		}*/
	}

	void DemoKeeper::createNewWindow()
	{
		MyGUI::Window* widget = MyGUI::LayoutManager::getInstance().loadLayout("Window.layout")[0]->castType<MyGUI::Window>();
		widget->eventWindowButtonPressed += MyGUI::newDelegate(this, &DemoKeeper::notifyWindowButtonPressed);

		mWidgets.insert(widget);
	}

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (_key == MyGUI::KeyCode::W)
		{
			createNewWindow();
		}
		else if (_key == MyGUI::KeyCode::V)
		{
			for (SetWidget::iterator item=mWidgets.begin(); item!=mWidgets.end(); ++item)
			{
				const MyGUI::IntCoord coord(0, 0, 1024, 768);
				const MyGUI::IntSize size(300, 300);

				if ((*item)->getCoord().width != coord.width)
				{
					(*item)->setCoord(coord);
				}
				else
				{
					(*item)->setCoord(coord.width / 2 - size.width / 2, coord.height / 2 - size.height / 2, size.width, size.height);
				}
			}
		}
		else if (_key == MyGUI::KeyCode::Four)
		{
			MyGUI::LayerManager::EnumeratorLayer layer = MyGUI::LayerManager::getInstance().getEnumerator();
			while (layer.next())
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
			while (layer.next())
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
							//	widget->setCaption(rtt ? "RTT mode" : "Vertex mode");


							/*for (SetWidget::iterator item=mWidgets.begin(); item!=mWidgets.end(); ++item)
							{
								(*item)->setCaption(mIsRTT ? "RTT mode" : "Vertex mode");
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
