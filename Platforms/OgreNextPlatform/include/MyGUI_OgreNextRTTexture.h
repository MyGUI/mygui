#ifndef MYGUI_OGRE_NEXT_RTTEXTURE_H_
#define MYGUI_OGRE_NEXT_RTTEXTURE_H_

#include "MyGUI_OgreNextPrerequisites.h"
#include "MyGUI_IRenderTarget.h"

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	// IRenderTarget backed by an offscreen Ogre::TextureGpu. Manages its own
	// RenderPassDescriptor and drives the same OgreNextManager batching that
	// the compositor pass uses.
	class OgreNextRTTexture : public IRenderTarget
	{
	public:
		explicit OgreNextRTTexture(Ogre::TextureGpu* texture);
		~OgreNextRTTexture() override;

		void begin() override;
		void end() override;
		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;
		const RenderTargetInfo& getInfo() const override
		{
			return mRenderTargetInfo;
		}

	private:
		void ensureRenderPassDescriptor();

		Ogre::TextureGpu* mTexture{nullptr};
		Ogre::RenderPassDescriptor* mRenderPassDesc{nullptr};
		RenderTargetInfo mRenderTargetInfo;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_RTTEXTURE_H_
