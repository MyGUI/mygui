#ifndef MYGUI_OGRE_NEXT_MANAGER_H_
#define MYGUI_OGRE_NEXT_MANAGER_H_

#include "MyGUI_OgreNextPrerequisites.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_ITexture.h"

#include <OgreMaterial.h>
#include <OgreMatrix4.h>
#include <OgreHlmsCommon.h>
#include <OgreTextureUnitState.h>

#include <vector>
#include <unordered_map>

#include "MyGUI_LastHeader.h"

namespace Ogre
{
	class Hlms;
}

namespace MyGUI
{
	// The middle layer between MyGUI's IRenderTarget flow and OgreNext's
	// v2 rendering. Owns the shader programs, a shared CommandBuffer + indirect
	// buffer, and a per-texture Renderable pool.
	//
	// One instance is created per OgreNextRenderManager::initialise.
	class OgreNextManager
	{
	public:
		explicit OgreNextManager(Ogre::SceneManager* scene);
		~OgreNextManager();

		// Begin one draw batch. Opens the render pass descriptor. Every
		// subsequent submitDraw call adds to the CommandBuffer. Must be
		// matched by endBatch.
		void beginBatch(Ogre::RenderPassDescriptor* rpd, Ogre::TextureGpu* target);

		// Suspend the current batch (ends it and saves state for later resume).
		// Returns true if a batch was active.
		bool suspendBatch();

		// Resume a previously suspended batch.
		// Only call after suspendBatch returned true.
		void resumeBatch();

		// Queue one MyGUI draw call into the current batch.
		void submitDraw(IVertexBuffer* buffer, ITexture* texture, size_t count);

		// Schedule a TUS samplerblock restore to run after endBatch() executes
		// the command buffer.  Used by doManualRender so the point-filter
		// samplerblock stays active during command-buffer execution.
		void deferSamplerRestore(Ogre::TextureUnitState* tu, const Ogre::HlmsSamplerblock* original);

		// Flush the CommandBuffer and close the current render pass descriptor.
		void endBatch();

		// Called by OgreNextTexture on destruction so we can release the
		// renderable cached for that TextureGpu.
		void notifyTextureDestroyed(Ogre::TextureGpu* tex);

		size_t getDrawCount() const
		{
			return mLastDrawCount;
		}

	private:
		void createPrograms();
		void createDummyMovable();
		void ensureIndirectBuffer(size_t neededDraws);
		void destroyIndirectBuffer();
		OgreNextRenderable* renderableFor(Ogre::TextureGpu* tex, const Ogre::MaterialPtr& material);

		struct SavedBatchState
		{
			Ogre::RenderPassDescriptor* rpd{nullptr};
			Ogre::TextureGpu* target{nullptr};
			Ogre::Matrix4 projMatrix;
			Ogre::HlmsCache passCache;
			Ogre::Hlms* hlms{nullptr};
			int baseInstanceAndIndirectBuffers{0};
		};

		Ogre::SceneManager* mScene{nullptr};
		Ogre::CommandBuffer* mCommandBuffer{nullptr};
		Ogre::IndirectBufferPacked* mIndirectBuffer{nullptr};
		Ogre::MovableObject* mDummyMovable{nullptr};

		using RenderableMap = std::unordered_map<Ogre::TextureGpu*, OgreNextRenderable*>;
		RenderableMap mRenderables;

		// Active batch state
		Ogre::RenderPassDescriptor* mActiveRPD{nullptr};
		Ogre::TextureGpu* mActiveTarget{nullptr};
		uint8_t* mIndirectMapped{nullptr};
		size_t mDrawIndex{0};
		size_t mIndirectCapacityDraws{0};

		Ogre::Matrix4 mActiveProjMatrix;
		Ogre::HlmsCache mPassCache;
		Ogre::Hlms* mActiveHlms{nullptr};
		int mBaseInstanceAndIndirectBuffers{0};

		size_t mLastDrawCount{0};

		// Nested batch support (for RTT texture rendering inside a batch)
		std::vector<SavedBatchState> mSavedBatchStates;

		// Pending sampler restores for doManualRender (processed in endBatch)
		// Keyed on TUS so only the first (original) state is kept per TUS.
		std::unordered_map<Ogre::TextureUnitState*, const Ogre::HlmsSamplerblock*> mPendingSamplerRestores;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_MANAGER_H_
