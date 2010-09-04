#include "precompiled.h"
#include "Layer.h"
#include "MyGUI_OgreRenderManager.h"

namespace demo
{
	Layer::Layer()
	{
	}

	Layer::~Layer()
	{
		destroyAllBuffers();
	}

	VertexBuffer* Layer::createVertexBuffer()
	{
		VertexBuffer* buffer = new VertexBuffer();
		mBuffers.push_back(buffer);
		return buffer;
	}

	void Layer::destroyAllBuffers()
	{
		for (VectorVertexBuffer::iterator item = mBuffers.begin(); item != mBuffers.end(); ++ item)
			delete (*item);
		mBuffers.clear();
	}

	void Layer::doRender(IRenderManager* _renderManager)
	{
		for (VectorVertexBuffer::iterator item = mBuffers.begin(); item != mBuffers.end(); ++ item)
		{
			if ((*item)->getEmpty())
				continue;

			(*item)->doRender(_renderManager);
		}
	}

} // namespace demo
