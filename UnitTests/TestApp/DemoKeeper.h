#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "Layer.h"
#include "IRenderManager.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseManager,
		public Ogre::RenderQueueListener,
		public IRenderManager
	{
	public:
		DemoKeeper();
		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void setupResources();

		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);

		virtual void initState();
		virtual Ogre::RenderSystem* getRenderSystem() { return mRenderSystem; }

		void doRender();

		Layer* createLayer();

		void destroyAllLayers();

	private:
		Ogre::RenderSystem* mRenderSystem;
		Ogre::LayerBlendModeEx mColourBlendMode;	//!< Controls colour blending mode used.
		Ogre::LayerBlendModeEx mAlphaBlendMode;	//!< Controls alpha blending mode used.
		Ogre::TextureUnitState::UVWAddressingMode mUvwAddressMode;

		typedef std::vector<Layer*> VectorLayer;
		VectorLayer mLayers;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
