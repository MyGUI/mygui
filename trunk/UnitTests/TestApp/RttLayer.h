#ifndef __RTT_LAYER_H__
#define __RTT_LAYER_H__

#include "Layer.h"

namespace demo
{
	class RttLayer :
		public Layer
	{
	public:
		RttLayer();
		virtual ~RttLayer();

		virtual void doRender(RenderManager* _renderManager);

	private:
		VertexBuffer* mBuffer;
		Ogre::TexturePtr mTexture;
		Ogre::Viewport* mViewport;
	};

} // namespace demo

#endif // __RTT_LAYER_H__
