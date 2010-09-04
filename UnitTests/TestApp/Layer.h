#ifndef __LAYER_H__
#define __LAYER_H__

#include "VertexBuffer.h"

namespace demo
{
	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		VertexBuffer* createVertexBuffer();
		void destroyAllBuffers();

		virtual void doRender(RenderManager* _renderManager);

	private:
		typedef std::vector<VertexBuffer*> VectorVertexBuffer;
		VectorVertexBuffer mBuffers;
	};

} // namespace demo

#endif // __LAYER_H__
