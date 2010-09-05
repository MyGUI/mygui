#ifndef __RENDER_MANAGER_H__
#define __RENDER_MANAGER_H__

#include <Ogre.h>
#include "Layer.h"

namespace demo
{
	class RenderManager :
		public Ogre::RenderQueueListener
	{
	public:
		RenderManager();
		virtual ~RenderManager();

		void initialise();
		void shutdown();

		bool getCurrentManual();
		void setCurrentManual(bool _value);

		void initState();
		Ogre::RenderSystem* getRenderSystem();

		Layer* createLayer();

		template <typename T>
		T* createLayer()
		{
			T* layer = new T();
			mLayers.push_back(layer);
			return layer;
		}

		void destroyAllLayers();

		void doRender();

	private:
		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);

		Ogre::SceneManager* getSceneManager();

	private:
		Ogre::LayerBlendModeEx mColourBlendMode;	//!< Controls colour blending mode used.
		Ogre::LayerBlendModeEx mAlphaBlendMode;	//!< Controls alpha blending mode used.
		Ogre::TextureUnitState::UVWAddressingMode mUvwAddressMode;

		bool mCurrentManual;
		Ogre::RenderSystem* mRenderSystem;

		typedef std::vector<Layer*> VectorLayer;
		VectorLayer mLayers;
	};

} // namespace demo

#endif // __RENDER_MANAGER_H__
