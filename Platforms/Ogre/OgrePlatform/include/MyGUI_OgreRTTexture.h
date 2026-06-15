/*!
	@file
	@author		Albert Semenov
	@date		10/2009
*/

#ifndef MYGUI_OGRE_RTTEXTURE_H_
#define MYGUI_OGRE_RTTEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IRenderTarget.h"

#include <OgrePrerequisites.h>
#include <OgreTexture.h>

namespace MyGUI
{

	class OgreRTTexture : public IRenderTarget
	{
	public:
		OgreRTTexture(Ogre::TexturePtr _texture);

		void begin() override;
		void end() override;

		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;

		const RenderTargetInfo& getInfo() const override
		{
			return mRenderTargetInfo;
		}

	private:
		RenderTargetInfo mRenderTargetInfo;
		Ogre::Viewport* mViewport{nullptr};
		Ogre::Viewport* mSaveViewport{nullptr};
		Ogre::TexturePtr mTexture;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_RTTEXTURE_H_
