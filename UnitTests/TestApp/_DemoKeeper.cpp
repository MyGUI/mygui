#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "RttLayer.h"

namespace demo
{
	DemoKeeper::DemoKeeper() :
		mRenderManager(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp");
	}

	void DemoKeeper::createScene()
	{
		mRenderManager = new RenderManager();
		mRenderManager->initialise();

		Layer* layer = mRenderManager->createLayer();
		VertexBuffer* buffer = layer->createVertexBuffer();
		buffer->setTextureName("smile1.png");
		buffer->setCoord(MyGUI::IntCoord(10, 10, 300, 300));
		buffer->setRenderMode(VertexBuffer::RenderModeFilterNone);

		buffer = layer->createVertexBuffer();
		buffer->setTextureName("smile2.png");
		buffer->setCoord(MyGUI::IntCoord(150, 150, 300, 300));
		//buffer->setRenderMode(VertexBuffer::RenderModePolygonWireframe);


		RttLayer* rttLayer = mRenderManager->createLayer<RttLayer>();
		buffer = rttLayer->createVertexBuffer();
		buffer->setTextureName("smile1.png");
		buffer->setCoord(MyGUI::IntCoord(10, 410, 300, 300));
		buffer->setRenderMode(VertexBuffer::RenderModeFilterNone);

		buffer = rttLayer->createVertexBuffer();
		buffer->setTextureName("smile2.png");
		buffer->setCoord(MyGUI::IntCoord(150, 550, 300, 300));


		rttLayer = mRenderManager->createLayer<RttLayer>();
		rttLayer->setTextureFilterNone(true);
		buffer = rttLayer->createVertexBuffer();
		buffer->setTextureName("smile1.png");
		buffer->setCoord(MyGUI::IntCoord(500, 410, 300, 300));

		buffer = rttLayer->createVertexBuffer();
		buffer->setTextureName("smile2.png");
		buffer->setCoord(MyGUI::IntCoord(650, 550, 300, 300));
		buffer->setRenderMode(VertexBuffer::RenderModeFilterNone);


		layer = mRenderManager->createLayer();
		buffer = layer->createVertexBuffer();
		buffer->setTextureName("smile1.png");
		buffer->setCoord(MyGUI::IntCoord(500, 10, 300, 300));

		buffer = layer->createVertexBuffer();
		buffer->setTextureName("smile2.png");
		buffer->setCoord(MyGUI::IntCoord(650, 150, 300, 300));
		buffer->setRenderMode(VertexBuffer::RenderModeFilterNone);
	}

	void DemoKeeper::destroyScene()
	{
		mRenderManager->shutdown();
		delete mRenderManager;
		mRenderManager = nullptr;
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
